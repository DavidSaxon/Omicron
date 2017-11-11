#version 130

in vec3 vertex_position;

uniform mat4 u_mvp_matrix;


void main()
{
    gl_Position = u_mvp_matrix * vec4(vertex_position, 1.0);
}
