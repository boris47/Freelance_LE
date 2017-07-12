SHADER version 1
@OpenGL2.Vertex
#version 400

uniform mat4 projectionmatrix;
uniform mat4 drawmatrix;
uniform vec2 offset;
uniform vec2 position[4];

in vec3 vertex_position;

void main( void ) {
	gl_Position = projectionmatrix * ( drawmatrix * vec4( position[ gl_VertexID ] + offset, 0.0, 1.0 ) );
}
@OpenGLES2.Vertex
@OpenGLES2.Fragment
@OpenGL4.Vertex
#version 400

uniform mat4 projectionmatrix;
uniform mat4 drawmatrix;
uniform vec2 offset;
uniform vec2 position[4];

void main( void ) {
	gl_Position = projectionmatrix * ( drawmatrix * vec4( position[ gl_VertexID ] + offset, 0.0, 1.0 ) );
}
@OpenGL4.Fragment
#version 400

// Engine Uniforms
uniform sampler2D texture1;
uniform sampler2DMS texture3;
uniform bool isbackbuffer;
uniform vec2 buffersize;
uniform vec2 camerarange;
uniform mat4 cameramatrix;
uniform mat4 projectioncameramatrix;
uniform float currenttime;

// Output color
out vec4 fragData0;

// User uniforms
uniform	int		iSunQuality			= 1;
uniform	float	fSunBright			= 1.0;
uniform vec3	vSunScreenPos		= vec3( 0.0 );
uniform vec3	vSunRotation		= vec3( 0.0 );
uniform	vec3	vSunColor			= vec3( 1.0 );
uniform	float	fLensFlareOpacity	= 1.0;

	#define GODRAYS
//	#define LENSFLARES
	#define SUNGLOW
	
	// Lens flare disable because i don't know hot to handle sun occlusion

// GodRays uniforms
uniform float	fExposure 		= 0.7;
uniform float	fDecay 			= 0.9;
uniform float 	fDensity		= 1.0;	// Ray length
uniform float	fGRThreshold	= 20; //500; //80;

// Sun glow prop
uniform float	fGlowSize		= 1.0;

#ifdef GODRAYS
float rand(vec2 co){
	return fract( sin ( dot ( co.xy, vec2( 12.9898, 78.233 ) ) ) * 43758.5453 );
}
float getnoise( vec2 pos ) {
	return abs( fract( sin( dot( pos, vec2( 18.9898f, 28.633f ) ) ) * 4378.5453f ) );
}
#endif

float DepthToZPosition( in vec2 vTexCoord ) {
	float depth = texelFetch( texture3, ivec2( vTexCoord * buffersize ), 0 ).r;
	return camerarange.x / ( camerarange.y - depth * ( camerarange.y - camerarange.x ) ) * camerarange.y;
}

vec4 GetPosition( in vec2 vTexCoord ) {
	float x = vTexCoord.s * 2.0f - 1.0f;
	float y = vTexCoord.t * 2.0f - 1.0f;
	float z = texelFetch( texture3, ivec2( vTexCoord * buffersize), 0 ).r;
	vec4 vProjPos = vec4( x, y, z, 1.0f );
	vec4 vViewPos = inverse( projectioncameramatrix ) * vProjPos;
	vViewPos /= vViewPos.w;
	return vViewPos;
}

#ifdef LENSFLARES
vec3 NewFlare( vec2 uv, vec2 pos ) {
	vec2 main	= uv - pos;
	vec2 uvd	= uv * length( uv );
	
	float ang	= atan( main.y, main.x );
	float dist	= pow( length( main ), .1 );
	float n		= getnoise( vec2( ( ang - ( currenttime / 9999.0 ) ) * 16.0, dist * 32.0 ) );
	// Big near rgb
	float f2r	= max( 1.0 / ( 1.0 + ( 32.0 * pow( length( uvd + 0.8  * pos ), 2.0 ) ) ), 0.0 ) * 00.25;
	float f2g	= max( 1.0 / ( 1.0 + ( 32.0 * pow( length( uvd + 0.85 * pos ), 2.0 ) ) ), 0.0 ) * 00.23;
	float f2b	= max( 1.0 / ( 1.0 + ( 32.0 * pow( length( uvd + 0.9  * pos ), 2.0 ) ) ), 0.0 ) * 00.21;
	
	// mid circle rgb
	vec2 uvx	= mix( uv, uvd, -0.5 );
	float f4r	= max( 0.01 - pow( length( uvx + 0.400 * pos ), 2.4 ), 0.0 ) * 6.0;
	float f4g	= max( 0.01 - pow( length( uvx + 0.450 * pos ), 2.4 ), 0.0 ) * 5.0;
	float f4b	= max( 0.01 - pow( length( uvx + 0.500 * pos ), 2.4 ), 0.0 ) * 3.0;
	// mid near rgb
	uvx			= mix( uv, uvd, -0.4 );
	float f5r	= max( 0.01 - pow( length( uvx + 0.200 * pos ), 5.5 ), 0.0 ) * 2.0;
	float f5g	= max( 0.01 - pow( length( uvx + 0.400 * pos ), 5.5 ), 0.0 ) * 2.0;
	float f5b	= max( 0.01 - pow( length( uvx + 0.600 * pos ), 5.5 ), 0.0 ) * 2.0;
	// small far rgb
	uvx			= mix( uv, uvd, -0.5 );
	float f6r	= max( 0.01 - pow( length( uvx - 0.300 * pos ), 1.6 ), 0.0 ) * 6.0;
	float f6g	= max( 0.01 - pow( length( uvx - 0.325 * pos ), 1.6 ), 0.0 ) * 3.0;
	float f6b	= max( 0.01 - pow( length( uvx - 0.350 * pos ), 1.6 ), 0.0 ) * 5.0;
	
	return vec3( f2r + f4r + f5r + f6r, f2g + f4g + f5g + f6g, f2b + f4b + f5b + f6b );
}

vec3 LensflareMix( vec3 color, float factor, float factor2 ) { // color modifier
	float w = color.x + color.y + color.z;
	return mix( color, vec3( w ) * factor, w * factor2 );
}
#endif

bool IsNearScreen( in vec2 vPos ) {
	return ( vPos.x > -0.5 && vPos.x < 1.5 && vPos.y > -0.5 && vPos.y < 1.5 );
}

bool IsInsideScreen( in vec2 vPos ) {
	return ( vPos.x > 0.0 && vPos.x < 1.0 && vPos.y > 0.0 && vPos.y < 1.0 );
}

/*
	vTexCoord  		[ 0 .. 1 ]
	buffersize 		[ 0 .. Width, 0.. Height ]
	gl_FragCoord.xy	[ 0 .. Width, 0.. Height ]
	
	texture( texture1, [ 0 .. 1, 0.. 1 ] );
	texelFetch( texture3, [ 0 .. Width, 0.. Height ] );
*/

void main() {
	
	vec2 vTexCoord = gl_FragCoord.xy / buffersize;
	if ( isbackbuffer ) vTexCoord.y = 1.0 - vTexCoord.y;
	
	fragData0 = texture( texture1, vTexCoord );
	
	// No glow, no party [ 0.0 .. 1.0 ]
	if ( fGlowSize == 0.0 ) return; // ( night )
	
	// Bright based on camera orientation [ 0.0 .. 1.0 ]
//	float fSunViewBright = clamp( distance( vSunScreenPos.xy, vec2( 0.5, 0.5 ) ), 0.0, 1.0 );
	float fSunViewBright = max( -vSunRotation.z, 0.0 );
	if ( fSunViewBright == 0.0 ) return; // ( sun outside camera view )
	
	float fAspectRatio = buffersize.x / buffersize.y;
	// Linear distance light - pixel
	float fSunDistanceFromPixel = sqrt( pow( ( vTexCoord.x * fAspectRatio ) - ( vSunScreenPos.x * fAspectRatio ), 2.0 ) + pow( vTexCoord.y - vSunScreenPos.y, 2.0 ) );
	// Glow factor for each pixel
	float fCorrectSunGlow = fGlowSize * 0.2;
	float fSunGlow = 1.0 - ( clamp( fSunDistanceFromPixel, 0.0, fCorrectSunGlow ) / fCorrectSunGlow );
	// Factor for god ray spread
	float fGRGlow = pow( 1.0 - clamp( fSunDistanceFromPixel, 0.0, 0.5 ), 2.0 );
	
	
	
#ifdef SUNGLOW
	// SUN GLOW
	// Actual pixel depth
	float fTexCoordDepth = DepthToZPosition( vTexCoord );
	if ( fTexCoordDepth > 250 ) {
		fragData0.rgb += vSunColor * fSunGlow;
	}
#endif
	
#ifdef GODRAYS
	// SUN GODRAYS
	
	int iGodRaySamples = 50;
	switch( iSunQuality ) {
		case 1: { iGodRaySamples = 70; break; }
		case 2: { iGodRaySamples = 100; break; }
		case 3: { iGodRaySamples = 150; }
	}
	
	vec2 vDeltaTexCoord = ( vTexCoord - vSunScreenPos.xy ) * ( fDensity / float( iGodRaySamples ) );
	vec2 vGodRayCoord = vTexCoord + vDeltaTexCoord;
	
	float fGodRay = 0.0;
	float fIlluminationDecay = 1.0;
	for ( int i = 0; i < iGodRaySamples; i++ ) {
		vGodRayCoord -= vDeltaTexCoord;
		fIlluminationDecay *= fDecay;
		
		if ( ( DepthToZPosition( vGodRayCoord ) > fGRThreshold ) ) {
			fGodRay += ( fExposure * fIlluminationDecay );
		}
	}
	
	if ( fGodRay > 0.0 ) {
		fGodRay /= iGodRaySamples;
		fragData0.rgb += ( vec3( 2.55, 1.41, 0.76 ) * ( 1.0 - fSunBright ) ) * ( fGodRay * vSunColor * fGRGlow );
	}
	
#endif

#ifdef LENSFLARES
	// SUN LENSFLARE.
/*
	float fSunVisibility = 0.0f;
	int iCount = 0;
	for( int i = 0; i < 16; i++ ) {
		for( int j = 0; j < 16; j++ ) {
			vec2 vDeltaCoord = ( vec2( i, j ) - 6 ) / 24; // [ -0.25  ..  0.25 ]
//			if ( !IsInsideScreen( vSunScreenPos.xy + vDeltaCoord ) ) { continue; } // skip
			
			if ( DepthToZPosition( vSunScreenPos.xy + vDeltaCoord ) > 50 )
				fSunVisibility += 1.0;
			iCount++;
		}
	}
	fSunVisibility /= float( iCount );
	*/
	vec2 uv = vTexCoord - 0.5;
	uv.x *= fAspectRatio;
	
	vec2 pos = vSunScreenPos.xy - 0.5;
	pos.x *= fAspectRatio;
	
	if ( DepthToZPosition( vSunScreenPos.xy ) > 50 ) {
		vec3 vFlareColor = vec3( 1.4, 1.2, 1.0 ) * NewFlare( uv, pos );
		vFlareColor = LensflareMix( vFlareColor, .7, .1 );
		
		fragData0.rgb += vFlareColor * vSunColor * 1.5;
	}
#endif
	
	fragData0.rgb = clamp( fragData0.rgb, 0.0, 1.0 );
}