#version 440
in vec2 coord;
uniform mat4 mvp;
void main()
{
  gl_Position = mvp * vec4(coord,0,1);
}
