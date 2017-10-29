#version 130

out vec4 out_colour;

uniform vec3 u_colour;

void main()
{
  out_colour = vec4(u_colour, 1.0);
}
