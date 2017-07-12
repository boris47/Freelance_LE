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

out vec4 fragData0;

const vec3 lumask = vec3(0.2125, 0.7154, 0.0721);
const float bright_add = 0.25;


void main(void)
{

	vec2 icoord = vec2(gl_FragCoord.xy/buffersize);
	if (isbackbuffer) icoord.y = 1.0 - icoord.y;
		
	const ivec2 add = ivec2(1,0);
	vec3 color = 0.25*(
		textureOffset(texture1, icoord, +add.xy)+
		textureOffset(texture1, icoord, -add.xy)+
		textureOffset(texture1, icoord, +add.yx)+
		textureOffset(texture1, icoord, -add.yx)
	).xyz;

	float bright = log(dot(color, lumask) + bright_add);
	color += 0.1*sin(-10.0*color/(color+vec3(2.0)));
	 
	fragData0 = vec4(color, bright);
}
