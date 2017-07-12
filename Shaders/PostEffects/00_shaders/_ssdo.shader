SHADER version 1
@OpenGL2.Vertex
#version 400

uniform mat4 projectionmatrix;
uniform mat4 drawmatrix;
uniform vec2 offset;
uniform vec2 position[4];

in vec3 vertex_position;

void main(void)
{
	gl_Position = projectionmatrix * (drawmatrix * vec4(position[gl_VertexID]+offset, 0.0, 1.0));
}
@OpenGLES2.Vertex

@OpenGLES2.Fragment

@OpenGL4.Vertex
#version 400

uniform mat4 projectionmatrix;
uniform mat4 drawmatrix;
uniform vec2 offset;
uniform vec2 position[4];

in vec3 vertex_position;

void main(void)
{
	gl_Position = projectionmatrix * (drawmatrix * vec4(position[gl_VertexID]+offset, 0.0, 1.0));
}
@OpenGL4.Fragment
#version 400
uniform bool isbackbuffer;

uniform vec2 buffersize;
uniform vec2 camerarange;
uniform float camerazoom;
uniform mat4 cameramatrix;

uniform sampler2D   texture1; //color
uniform sampler2DMS texture2; //depth

out vec4 fragData0;

#define raycasts 5//Increase for better quality
#define raysegments 2//Increase for better quality
#define aoscatter 0.4
#define raylength 0.25
#define cdm .5

float DepthToZPosition(in float depth) {
	return camerarange.x / (camerarange.y - depth * (camerarange.y - camerarange.x)) * camerarange.y;
}

float readZPosition(in vec2 texcoord) {
	return DepthToZPosition( texelFetch( texture2, ivec2((texcoord + 0.25 / buffersize)*buffersize),0 ).x );
}

mat3 vec3tomat3( in vec3 z ) {
	mat3 mat;
	mat[2]=z;
	vec3 v=vec3(z.z,z.x,-z.y);//make a random vector that isn't the same as vector z
	mat[0]=cross(z,v);//cross product is the x axis
	mat[1]=cross(mat[0],z);//cross product is the y axis
	return mat;
}

float compareDepths( in float depth1, in float depth2, in float m ) {
	float mindiff=.05*m;
	float middiff=.5*m;
	float maxdiff=1.5*m+(1-1*clamp(depth2,0.,1.));
	float enddiff=2.0*m+(3-3*clamp(depth2,0.,1.));
	
	float diff = (depth1-depth2);
	if (diff<mindiff) {
		return 1.0;
	}
	if (diff<middiff) {
		diff -= mindiff;
		return 1.0-diff/(middiff-mindiff);
	}
	if (diff<maxdiff) {
		return 0.0;
	}
	if (diff<enddiff) {
		diff -= maxdiff;
		return diff/(enddiff-maxdiff);
	}
	return 1.0;	
}


vec3 ScreenCoordToPosition2( in vec2 coord, in float z ) {
	vec3 screencoord;
	screencoord = vec3(((coord.x)-0.5) * 2.0,((coord.y)-0.5) * 2.0 / (buffersize.x/buffersize.y),( z ));
	screencoord.x *= screencoord.z;
	screencoord.y *= -screencoord.z;
	return screencoord;
}

//Converts a screen position to texture coordinate
vec2 ScreenPositionToCoord( in vec3 pos ) {
	vec2 coord;
	vec2 hbuffersize=buffersize/2.0;
	pos.x /= (pos.z / camerazoom);
	coord.x = (pos.x / 2.0 + 0.5);
	
	pos.y /= (-pos.z / camerazoom) / (hbuffersize.x/hbuffersize.y);
	coord.y = -(pos.y / 2.0 - 0.5);
	
	return coord;// + 0.5/(buffersize/2.0);
}

float rand(vec2 co) {
        return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main( void ) {
	float ao=0.0;
	float dn;
	float zd;
	float newdepth;
	float angletest;
	float lineardepth;
	float depth;
	vec3 newnormal;
	vec3 newdiffuse;
	vec3 normal;
	vec2 texcoord2;
	vec2 texcoord = gl_FragCoord.xy/buffersize + 0.5/(buffersize*0.5);
	if (isbackbuffer) texcoord.y = 1.0 - texcoord.y;

	// this save resources
	if ( cameramatrix[3][1] <= -0.99 ) {
		fragData0 = texture( texture1, texcoord );
		return;
	}
	
	depth = texelFetch( texture2, ivec2( texcoord * buffersize ), 0 ).x;
	vec3 screencoord;
	vec4 outcolor;
	
	if (depth<1.0) {
		
		lineardepth = DepthToZPosition(depth);
		
		vec3 p0=ScreenCoordToPosition2(vec2(0.0,0.5),lineardepth);
		vec3 p1=ScreenCoordToPosition2(vec2(1.0,0.5),lineardepth);
		float dist = abs(p1.x-p0.x);
		
		screencoord = vec3((((gl_FragCoord.x+0.5)/buffersize.x)-0.5) * 2.0,(((1-gl_FragCoord.y+0.5)/buffersize.y)+0.5) * 2.0 / (buffersize.x/buffersize.y),lineardepth);
		screencoord.x *= screencoord.z / camerazoom;
		screencoord.y *= -screencoord.z / camerazoom;
		if (isbackbuffer) screencoord.y *= -1.0;
		normal = normalize(screencoord);
		
		vec3 newpoint;
		vec2 coord;
		vec3 raynormal;
		vec3 offsetvector;
		float diff;		
		vec2 randcoord;	
		mat4 cameramat4=cameramatrix;
		float randsum = cameramat4[0][0]+cameramat4[0][1]+cameramat4[0][2];
		randsum+=cameramat4[1][0]+cameramat4[1][1]+cameramat4[1][2];
		randsum+=cameramat4[2][0]+cameramat4[2][1]+cameramat4[2][2];
		randsum+=cameramat4[3][0]+cameramat4[3][1]+cameramat4[3][2];
		

		
		ao=0.0;
		
		vec4 gicolor;
		float gisamples;
		mat3 mat=vec3tomat3(normal);
		float a;
		float wheredepthshouldbe;
		
		float mixs=(1.0-(clamp(lineardepth-100.0,0.0,100.0)/100.0));
		
		//Get a random number
		a=rand( randsum+texcoord);// + float(53.0*m*raysegments + i*13.0) );
		
		vec4 gi=vec4(0);
		float aof;
		float gisum;
		
		if (mixs>0.0) {

			for ( int i=0;i<(raycasts);i++ ) {
				for ( int m=0;m<(raysegments);m++ ) {
					offsetvector.x=cos(a+float(i)/float(raycasts)*3.14*4.0)*aoscatter;
					offsetvector.y=sin(a+float(i)/float(raycasts)*3.14*4.0)*aoscatter;
					offsetvector.z=1.0;
					offsetvector=normalize(offsetvector);
					
					//Create the ray vector
					raynormal=mat*offsetvector;
					
					//Add the ray vector to the screen position
					newpoint = screencoord + raynormal * (raylength/raysegments) * float(m+1);
					wheredepthshouldbe=newpoint.z;
					
					//Turn the point back into a screen coord:
					coord = ScreenPositionToCoord( newpoint );
					
					//Look up the depth value at that rays position
					newdepth = readZPosition( coord );
					
					ao += compareDepths( wheredepthshouldbe,newdepth, cdm );
				}
			}
		}
		
		gi /= gisum;
		
		ao /= (raycasts*raysegments);
		ao = ao * mixs + (1.0-mixs);	
		ao += clamp(lineardepth,0.0,100.0)/100.0;
		
		fragData0=vec4(clamp(ao+ao+ao*.25,0.,1.));
	}
	
	else {
		fragData0=vec4(1.0);
	}
	fragData0*=1.0*texture(texture1,texcoord);
}
