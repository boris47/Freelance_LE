SHADER version 1
@OpenGL2.Vertex
#version 400

uniform mat4 projectioncameramatrix;
uniform mat4 drawmatrix;
uniform vec2 offset;
uniform vec2 position[4];
uniform vec3 lightglobalposition;
uniform vec2 lightrange;
uniform vec2 lightconeangles;
uniform mat4 entitymatrix;
uniform mat4 camerainversematrix;

in vec3 vertex_position;

out vec4 vposition;

void main(void)
{
	vec4 pos = entitymatrix * vec4(vertex_position,1.0);
	vposition = camerainversematrix * pos;
	gl_Position = projectioncameramatrix * pos;
	/*
	vec3 position = vertex_position;
	position.x *= lightrange.y * tan(lightconeangles[1]);
	position.y *= lightrange.y;
	position.z *= lightrange.y * tan(lightconeangles[1]);
	gl_Position = projectioncameramatrix * vec4(lightglobalposition + position,1.0);*/
}
@OpenGLES2.Vertex

@OpenGLES2.Fragment

@OpenGL4.Vertex
#version 400

uniform mat4 projectioncameramatrix;
uniform mat4 drawmatrix;
uniform vec2 offset;
uniform vec2 position[4];
uniform vec3 lightglobalposition;
uniform vec2 lightrange;
uniform vec2 lightconeangles;
uniform mat4 entitymatrix;
uniform mat4 camerainversematrix;

in vec3 vertex_position;

out vec4 vposition;

void main(void)
{
	vec4 pos = entitymatrix * vec4(vertex_position,1.0);
	vposition = camerainversematrix * pos;
	gl_Position = projectioncameramatrix * pos;
	/*
	vec3 position = vertex_position;
	position.x *= lightrange.y * tan(lightconeangles[1]);
	position.y *= lightrange.y;
	position.z *= lightrange.y * tan(lightconeangles[1]);
	gl_Position = projectioncameramatrix * vec4(lightglobalposition + position,1.0);*/
}
@OpenGL4.Fragment
#version 400
#ifndef SAMPLES
	#define SAMPLES 1
#endif
#define PI 3.14159265359
#define HALFPI PI/2.0
#define LOWERLIGHTTHRESHHOLD 0.001
#ifndef KERNEL
	#define KERNEL 3
#endif
#define KERNELF float(KERNEL)
#define GLOSS 10.0
#define VOLUMETRICLIGHTING 1

uniform sampler2DMS texture0;
uniform sampler2DShadow texture5;//shadowmap
uniform vec2 lightconeanglescos;
uniform vec2 buffersize;

uniform vec3 lightposition;
uniform vec3 lightdirection;
uniform vec4 lightcolor;
uniform vec4 lightspecular;
uniform vec2 lightrange;

uniform vec2 camerarange;
uniform float camerazoom;
uniform mat3 lightnormalmatrix;
uniform vec2 lightshadowmapoffset;
uniform float shadowsoftness;
uniform bool isbackbuffer;
uniform float volumetricintensity=1.0;
uniform int downsampling=1;

in vec4 vposition;

out vec4 fragData0;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

float depthToPosition(in float depth, in vec2 depthrange)
{
	return depthrange.x / ( depthrange.y - depth * ( depthrange.y - depthrange.x ) ) * depthrange.y;
}

float positionToDepth(in float z, in vec2 depthrange) {
	return ( depthrange.x / ( z / depthrange.y ) - depthrange.y ) / -( depthrange.y - depthrange.x );
}

void main(void)
{
	vec3 flipcoord = vec3(1.0);
	if ( !isbackbuffer ) flipcoord.y = -1.0;
	
	//----------------------------------------------------------------------
	//Calculate screen texcoord
	//----------------------------------------------------------------------
	ivec2 icoord = ivec2( gl_FragCoord.xy );
	if ( isbackbuffer ) icoord.y = int( buffersize.y ) - icoord.y;
	icoord *= downsampling;
	
	//----------------------------------------------------------------------
	//Retrieve data from gbuffer
	//----------------------------------------------------------------------
	float depth = 		texelFetch( texture0, icoord, 0 ).x;
	
	//----------------------------------------------------------------------
	//Calculate screen position and vector
	//----------------------------------------------------------------------
	vec3 screencoord = vec3(
		( (  gl_FragCoord.x / buffersize.x ) - 0.5 ) * 2.0 * ( buffersize.x / buffersize.y ),
		( ( -gl_FragCoord.y / buffersize.y ) + 0.5 ) * 2.0,
		depthToPosition( depth, camerarange )
	);
	screencoord.x *=  screencoord.z / camerazoom;
	screencoord.y *= -screencoord.z / camerazoom;
	vec3 screennormal = normalize( screencoord );
	
#if VOLUMETRICLIGHTING==1
	//Volumetric lighting
	screencoord 	*= flipcoord;
	screennormal 	*= flipcoord;
	if ( vposition.z < screencoord.z ) screencoord = vposition.xyz;
	
	int 	iSunQuality 	= int( lightspecular.a ); // 2 or 3
	
	/*
	float stepsize = 0.1;//lightrange.y * 2.0 / float(steps);
	int steps = int(lightrange.y * 2.0 / stepsize + 0.5);		
	*/
	
	float 	stepsize 		= 0.30;  // 0.15
	int 	steps 			= 15 * iSunQuality; // 60 or 90
	
	
	float 	vlighting 		= 0.0;
	float 	lightdistance 	= length( lightposition );
	vec3 	lightvector;
	vec3 	shadowcoord;
	float 	attenuation;
	vec3 	lightnormal;
	
	vec3 dithercoord = lightnormalmatrix * screennormal;
	vec3 pos = screencoord + screennormal * stepsize * rand( dithercoord.xy * dithercoord.z );
	
	for ( int n = 0; n < steps; n++ ) {
		pos -= screennormal * stepsize;
		if ( lightdistance > lightrange.y || pos.z < camerarange.x ) {
			if (length( pos ) < ( lightdistance - lightrange.y ) || pos.z < camerarange.x ) break;// slower?
		}
		
		float dist = length( lightposition - pos );
		if ( dist < lightrange.y && pos.z > camerarange.x ) {
			attenuation = 1.0 - dist / lightrange.y;
			lightvector = pos - lightposition;
			lightnormal = normalize( lightvector );
			
			//Spot cone attenuation
			float denom = lightconeanglescos.y - lightconeanglescos.x;	
			float anglecos = dot( lightnormal, lightdirection );
			
			if ( denom > 0.0 )
			{		
				attenuation *= 1.0 - clamp( ( lightconeanglescos.y - anglecos ) / denom, 0.0, 1.0 );
			}
			else
			{
				if ( anglecos < lightconeanglescos.x ) attenuation = 0.0;
			}
#ifdef USESHADOW
			if ( attenuation > 0.0 )
			{
				shadowcoord = lightnormalmatrix * lightvector;
				shadowcoord.x /= -shadowcoord.z / 0.5;
				shadowcoord.y /=  shadowcoord.z / 0.5;
				shadowcoord.xy += 0.5;
//				shadowcoord.z += 0.9; // my fix
				shadowcoord.z = positionToDepth( ( shadowcoord.z * lightshadowmapoffset.y ) - lightshadowmapoffset.x, lightrange );
				attenuation *= texture( texture5, shadowcoord );
			}
#endif
			attenuation = max( 0.0, attenuation );
			vlighting += volumetricintensity * 0.1 * stepsize * attenuation;
		}
	}
	vlighting = min( volumetricintensity, vlighting );
	fragData0 = vec4( lightspecular.rgb, 1.0 ) * vlighting;
#endif
}
