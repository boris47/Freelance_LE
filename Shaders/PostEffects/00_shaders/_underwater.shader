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
uniform float currenttime;

out vec4 fragData0;
out vec4 fragData1;

uniform sampler2D texture1;
uniform sampler2DMS texture2;
uniform sampler2DMS texture3;
uniform sampler2D texture4;
uniform vec3 cameraposition;
uniform mat4 camerainversematrix;
uniform mat4 cameramatrix;
uniform mat4 drawmatrix;
uniform mat4 projectioncameramatrix;
uniform mat4 projectionmatrix;
uniform mat3 cameranormalmatrix;
uniform mat4 inversematrix;
uniform float camerazoom;
uniform mat4 camerainverse;

uniform bool isunderwater=false;
uniform bool useshafts=false;
uniform float level=0.0;

uniform vec3 eye;
//uniform vec3 shaftpos=vec3(.5,.5,1.);

uniform vec2 fogrange = vec2(-50.0,50.0);
uniform vec4 fogcolor = vec4(0.02,0.3,0.37,1.0);
uniform vec2 fogangle  = vec2(0.0,15.0);
uniform bool fogislocal = false;
 
float rand(int seed, float ray) {
	return mod(sin(float(seed)*363.5346+ray*674.2454)*6743.4365, 1.0);
} 

float depthToPosition(in float depth, in vec2 depthrange)
{
	return depthrange.x / (depthrange.y - depth * (depthrange.y - depthrange.x)) * depthrange.y;
}

vec4 GetShafts(vec2 pos)
{
	const float pi = 3.14159265359;
	vec4 color;
	vec2 position = ( gl_FragCoord.xy / buffersize.xy ) -  pos;
	position.y *= buffersize.y/buffersize.x;
	float ang = atan(position.y, position.x);
	float dist = length(position);
	color.rgb = vec3(0.4, 0.95, 1.15) * (pow(dist, -1.0) * 0.001);
	for (float ray = 0.0; ray < 8.0; ray += 0.095) {
		float rayang = rand(5, ray)*6.2+(currenttime*0.0001)*20.0*(rand(2546, ray)-rand(5785, ray))-(rand(3545, ray)-rand(5467, ray));
		rayang = mod(rayang, pi*2.0);
		if (rayang < ang - pi) {rayang += pi*2.0;}
		if (rayang > ang + pi) {rayang -= pi*2.0;}
		float brite = .5 - abs(ang - rayang);
		brite -= dist * 0.3;
		if (brite > 0.0) {
			color.rgb += vec3(0.1+0.4*rand(8644, ray), 0.55+0.4*rand(4567, ray), 0.7+0.4*rand(7354, ray)) * brite * 0.1;
		}
	}
	color.a = 1.0;
	return color;
}

vec3 PositionTex()
{
	vec2 coord = gl_FragCoord.xy / buffersize;
	if (isbackbuffer) coord.y = 1.0 - coord.y;
	
	ivec2 icoord = ivec2(gl_FragCoord.xy);
	if (isbackbuffer) icoord.y = int(buffersize.y) - icoord.y;
		float depth = 		texelFetch(texture2,icoord,0).x;
		vec3 screencoord = vec3(((gl_FragCoord.x/buffersize.x)-0.5) * 2.0 * (buffersize.x/buffersize.y),((-gl_FragCoord.y/buffersize.y)+0.5) * 2.0,depthToPosition(depth,camerarange));
		screencoord.x *= screencoord.z / camerazoom;
		screencoord.y *= -screencoord.z / camerazoom;
		vec3 screennormal = normalize(screencoord);
		if (!isbackbuffer) screencoord.y *= -1.0;
		
		return screencoord;
}

void main()
{
	vec2 coord = gl_FragCoord.xy / buffersize;
	if (isbackbuffer) coord.y = 1.0 - coord.y;
	
	float aspect = buffersize.y/buffersize.x;
			
	float depth = texelFetch(texture2,ivec2(coord*buffersize),0).x;
	float tdepth=depth;
	float currDepth = depthToPosition(depth,camerarange);//linearizeDepth( texture(deferredDepthTex, vert_UV).z );

	
	//normalize it to get the cubecoords
	vec3 screencoord;
	screencoord = vec3(((coord.x)-0.5) * 2.0,((coord.y)-0.5) * 2.0 / (buffersize.x/buffersize.y),currDepth);
	screencoord.x *= screencoord.z;
	screencoord.y *= -screencoord.z;
	vec4 worldpostemp= vec4(screencoord,1.0) * camerainversematrix;
	vec3 worldpos = worldpostemp.xyz;// / 1.0-worldpostemp.w;
	worldpos+=cameraposition;
	vec3 cubecoord = normalize(worldpos.xyz);
	
	vec4 objectscreenNormal = texelFetch(texture3,ivec2(coord*buffersize),0)*2-1;
	objectscreenNormal=normalize(objectscreenNormal);
	objectscreenNormal*=camerainversematrix;
	if (tdepth==1.0) objectscreenNormal.y=1;

	vec3 flipcoord = vec3(1.0);
	if (!isbackbuffer) flipcoord.y = -1.0;

	vec3 position=PositionTex();
		
	vec4 p=vec4(position,1)*camerainversematrix;
	position=level-p.xyz;

	vec3 eyeVec = position - cameraposition;
	float dif = level - position.y;
	float cameraDepth = cameraposition.y - position.y+1;

	vec3 eyeVecNorm = normalize(eyeVec);
	float t = (level - cameraposition.y) / eyeVecNorm.y;
	vec3 surfacePoint = cameraposition + eyeVecNorm * t;

	eyeVecNorm = normalize(eyeVecNorm);

	depth = length(position - surfacePoint);
	float depth2 = surfacePoint.y - position.y;
				
	vec3 waterPosition = surfacePoint.xyz;
	
	vec2 scroll=eyeVec.xz;
	
	float ds=clamp(1-.01*(-cameraposition.y+level),0.2,1.);
	
	float causticsize=(1)/clamp(abs(floor(cameraDepth*cameraDepth)),2.,50.);
	
	scroll.x=eyeVec.x*causticsize+(currenttime/12500)+.1;//+eyeVec.z;
	scroll.y=eyeVec.z*causticsize+(currenttime/12500);//+eyeVec.x;
	
	vec4 caustics=2.0 * texture(texture4, scroll.xy) * ((.95*clamp(-cameraDepth/10.+1.5,0.,1.)));
	
	scroll.x=eyeVec.x*causticsize-(currenttime/12500);//+eyeVec.z+.1;
	scroll.y=eyeVec.z*causticsize-(currenttime/12500);//+eyeVec.x;
	
	caustics*=texture(texture4, scroll.xy) * ((.95*clamp(-cameraDepth/10.+1.5,0.,1.)))* 2.0;
	caustics*=clamp(2.*objectscreenNormal.y,0.,1.);
	

	if (isunderwater) // no background - render input + fog to output
	{
		fragData0=texture(texture1, coord)*clamp((1+cameraDepth/40),0.2,1.)*vec4(0.7,1,1,1);
		fragData0+=.5*caustics*(clamp((1.+cameraDepth/20.),0.,1.));
		if(tdepth == 1.0) //no geometry rendered --> background
		{		
			vec3 normal=normalize(cubecoord);
			normal.y=max(normal.y,0.0);
			float angle=asin(normal.y)*57.2957795-fogangle.x;
			float fogeffect=1.0-clamp(angle/(fogangle.y-fogangle.x+20.*max(2.,-cameraposition.y+level)),0.0,1.0);
			fogeffect *= fogcolor.w;
			fragData0=fragData0*(1.0-fogeffect)+fogeffect*fogcolor * ds;
			}
		else // no background - render input + fog to output
		{
			float lineardepth = currDepth;
			float fogeffect = 0.;
			fogeffect = clamp( 1.0 - (fogrange.y - lineardepth) / (fogrange.y - fogrange.x) , 0.0, 1.0 );
			fragData0 = (fragData0 * (1.0 - fogeffect) + fogcolor * fogeffect * ds);	
		}
		//vec2 shaftposition=shaftpos.xy/buffersize;
		if (useshafts) fragData0+=.75*ds*GetShafts(vec2(.5,-1));
	}
	else
	{
		fragData0=texture(texture1, coord);//*vec4(0,1,1,1);
		if (cameraDepth < 1)
			fragData0+=.25*caustics*(clamp((1.+cameraDepth),0.,1.));
	}
	//if (cameramatrix[3][1] <= -0.99) fragData0=texture(texture1, coord);
	//fragData0*=vec4(cameramatrix[3][1]);
}
