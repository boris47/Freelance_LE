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
uniform float appspeed;

out vec4 fragData0;

void main(void)
{

vec2 icoord = vec2(gl_FragCoord.xy/buffersize);
if (isbackbuffer) icoord.y = 1.0 - icoord.y;

// Sample the average upwards screen brightness
vec4
/*exposurecolor = texture(texture1,vec2(0.0,0.0)	+vec2(.5,.5));

exposurecolor += texture(texture1,vec2(-0.1,0.0)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.1,0.0)	+vec2(.5,.5));

exposurecolor += texture(texture1,vec2(0.0,0.1)	+vec2(.5,.5));

exposurecolor += texture(texture1,vec2(-0.1,-0.1)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.0,-0.1)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.1,-0.1)+vec2(.5,.5));

exposurecolor += texture(texture1,vec2(-0.2,-0.2)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(-0.1,-0.2)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.0,-0.2)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.1,-0.2)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.2,-0.2)+vec2(.5,.5));

exposurecolor += texture(texture1,vec2(-0.3,-0.3)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(-0.2,-0.3)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(-0.1,-0.3)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.0,-0.3)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.1,-0.3)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.2,-0.3)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.3,-0.3)+vec2(.5,.5));*/

exposurecolor = texture(texture1,vec2(-0.4,-0.4)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(-0.2,-0.3)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(-0.1,-0.4)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.0,-0.3)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.1,-0.4)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.2,-0.3)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.4,-0.4)+vec2(.5,.5));

exposurecolor += texture(texture1,vec2(-0.4,-0.2)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(-0.2,-0.3)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(-0.1,-0.2)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.0,-0.3)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.1,-0.2)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.2,-0.3)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.4,-0.2)+vec2(.5,.5));

exposurecolor += texture(texture1,vec2(-0.4,-0.0)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(-0.2,-0.1)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(-0.1,-0.0)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.0,-0.1)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.1,-0.0)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.2,-0.1)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.4,-0.0)+vec2(.5,.5));

exposurecolor += texture(texture1,vec2(-0.4,0.1)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(-0.2,0.2)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(-0.1,0.1)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.0,0.2)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.1,0.1)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.2,0.2)+vec2(.5,.5));
exposurecolor += texture(texture1,vec2(0.4,0.1)+vec2(.5,.5));

exposurecolor /= 28.0;	

// Get the average luminance
float avgLuminance = exposurecolor.r * 0.3 + exposurecolor.g * 0.59 + exposurecolor.b * 0.11;

// Here's the tone mapping calculation.  I wanted the exposure to never be darker than the normal
// light level.  I didn't want the exposure to fluctuate when in the bright range, but I wanted
// dark lighting to have a strong effect on the exposure.  A few dark objects on the screen shouldn't make the 
// exposure lighter, but in a dark room a bright window will have a visible effect.  The following equation
// provides the behavior I was looking for:

float irisadjustment = 1.0 / (avgLuminance/0.25);
irisadjustment = clamp(irisadjustment,0.0,2.0);
vec4 c=texture(texture1, icoord) * irisadjustment ;
fragData0=vec4(irisadjustment);
fragData0.a=.01*(1+appspeed);
//fragData0=exposurecolor;
}
