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

uniform sampler2D texture0;
uniform sampler2D texture1;
uniform bool isbackbuffer;
uniform sampler2D texture9;
uniform vec2 buffersize;
uniform float currenttime;
uniform mat4 cameramatrix;

out vec4 fragData0;

uniform float Luminance = 0.1;
uniform float fMiddleGray = 0.48;
uniform float fWhiteCutoff = 0.98;

//-----------------------------------------------------------------------------
// Pixel Shader: BrightPassFilter
// Desc: Perform a high-pass filter on the source texture
//-----------------------------------------------------------------------------
vec4 BrightPassFilter( vec4 c)
{
    vec3 ColorOut = c.rgb;

    ColorOut *= fMiddleGray / ( Luminance + 0.001f );
    ColorOut *= ( 1.0f + ( ColorOut / ( fWhiteCutoff * fWhiteCutoff ) ) );
    ColorOut -= 5.0f;
    ColorOut = max( ColorOut, 0.0f );
    ColorOut /= ( 10.0f + ColorOut );
    return vec4( ColorOut, 1.0f );
}



void main(void)
{

vec2 icoord = vec2(gl_FragCoord.xy/buffersize);
if (isbackbuffer) icoord.y = 1.0 - icoord.y;
	
vec4 color = BrightPassFilter(texture(texture1, icoord)); // glowmap
fragData0 = vec4(color.rgb, 1.0);
}
