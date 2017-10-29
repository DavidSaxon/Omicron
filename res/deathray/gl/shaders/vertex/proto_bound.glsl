#version 130

in vec3 in_position;

uniform mat4 u_vp_matrix;


void main()
{
    vec4 position = vec4(in_position, 1.0);
    gl_Position = u_vp_matrix * position;
}
