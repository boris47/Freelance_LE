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

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2DMS texture3;

uniform float gain=1.25;
uniform float expose=1.0;
uniform vec2 clampiris=vec2(0.0,2.0);

const vec3 lumask = vec3(0.2125, 0.7154, 0.0721);
const float bright_add = 0.25;

void main() 
{

	vec2 icoord = vec2(gl_FragCoord.xy/buffersize);
	if (isbackbuffer) icoord.y = 1.0 - icoord.y;
	
	const ivec2 add = ivec2(1,0);
	vec3 ccolor = 0.25*(
		textureOffset(texture2, icoord, +add.xy)+
		textureOffset(texture2, icoord, -add.xy)+
		textureOffset(texture2, icoord, +add.yx)+
		textureOffset(texture2, icoord, -add.yx)
	).xyz;

	float bright = log(dot(ccolor, lumask) + bright_add);
	
	vec4 exposure=texture(texture1,icoord);
	float e=(exposure.r+exposure.g+exposure.b)/3.0;
	e = (expose*e+clamp(pow(max(e,.25),gain),0.,.5));
	e=clamp(e,clampiris.x,clampiris.y);
	ccolor += (1-e)*sin(-5*ccolor/(ccolor+vec3(2.0)));
	fragData0=vec4(ccolor,1);
	//fragData0=exposure*.5+(vec4(ccolor*.5,1));
}
