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
//--------------------------------------
// Thermal shader by Shadmar
//--------------------------------------

#version 400

uniform sampler2D texture1;
uniform bool isbackbuffer;
uniform vec2 buffersize;
uniform float currenttime;

out vec4 fragData0;

float vx_offset=1.01;
 
void main() 
{ 

vec2 icoord = vec2(gl_FragCoord.xy/buffersize);
if (isbackbuffer) icoord.y = 1.0 - icoord.y;
  
float noise =  (fract(sin(dot(icoord ,vec2(12.0,78.0)+currenttime)) * 43758.0));

  vec3 tc = vec3(1.0, 0.0, 0.0);
  if (icoord.x < (vx_offset-0.005))
  {
    vec3 pixcol = texture(texture1, icoord).rgb;
    vec3 colors[3];
    colors[0] = vec3(0.,0.,1.);
    colors[1] = vec3(1.,1.,0.);
    colors[2] = vec3(1.,0.,0.);
    float lum = (pixcol.r+pixcol.g+pixcol.b)/3.;
    int ix = (lum < 0.5)? 0:1;
    tc = mix(colors[ix],colors[ix+1],(lum-float(ix)*0.5)/0.5);
  }
  else if (icoord.x>=(vx_offset+0.005))
  {
    tc = texture(texture1, icoord).rgb;
  }
	fragData0 = vec4(tc-noise*0.1, 1.0);
}
