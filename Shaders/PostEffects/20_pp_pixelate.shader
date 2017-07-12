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

uniform sampler2D texture1;
uniform bool isbackbuffer;
uniform vec2 buffersize;

out vec4 fragData0;

float downgrade = 6.0; //at original 1920x1080, 6 yields  320x180
void main(void)
{
 	vec4 modula = mod(gl_FragCoord, downgrade);
	vec2 newCoords = vec2(gl_FragCoord - modula) / buffersize.xy;
	if (isbackbuffer) newCoords.y = 1.0 - newCoords.y;
	fragData0 = texture(texture1, newCoords);
}
