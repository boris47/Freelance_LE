SHADER version 1
@OpenGL2.Vertex
#version 400

uniform mat4 projectionmatrix;
uniform mat4 drawmatrix;
uniform vec2 offset;
uniform vec2 position[4];
uniform mat4 projectioncameramatrix;
uniform mat4 camerainversematrix;
uniform vec3 cameraposition;

in vec3 vertex_position;

void main() {
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
uniform mat4 projectioncameramatrix;
uniform mat4 camerainversematrix;
uniform vec3 cameraposition;

in vec3 vertex_position;

void main() {
    gl_Position = projectionmatrix * (drawmatrix * vec4(position[gl_VertexID]+offset, 0.0, 1.0));
}

@OpenGL4.Fragment
#version 400

// Engine Uniforms
uniform sampler2D texture1;
uniform sampler2DMS texture3;
uniform bool isbackbuffer;
uniform vec2 buffersize;
uniform vec2 camerarange;

// User uniforms
uniform int		IsActive	= 1;
uniform float 	fFogRange 	= 80.0;
uniform vec3 	vFogColor 	= vec3( 0.17f, 0.13f, 0.15f );
uniform float 	fFogDensity	= .9;

// Output color
out vec4 fragData0;

float DepthToZPosition( in vec2 vTexCoord ) {
	float depth = texelFetch( texture3, ivec2( vTexCoord * buffersize ), 0 ).r;
	return camerarange.x / ( camerarange.y - depth * ( camerarange.y - camerarange.x ) ) * camerarange.y;
}

void main() {
	
	vec2 vTexCoord = vec2( gl_FragCoord.xy / buffersize );
	if ( isbackbuffer ) vTexCoord.y = 1.0 - vTexCoord.y;
	
	fragData0 = texture( texture1, vTexCoord );
	
	if ( IsActive == 0 ) return;
	
	vec3 vEffColor = vFogColor * ( fFogDensity * 10.f );
	
	float fDepth = DepthToZPosition( vTexCoord );
	if ( fDepth < fFogRange ) {
		float fFogFactor = clamp( ( fFogRange - fDepth ) / fFogRange, 0.0, 1.0 );
		fragData0.rgb += vEffColor  * ( 1.0 - fFogFactor );
		return;
	}
	
	fragData0.rgb += vEffColor;
	
}
