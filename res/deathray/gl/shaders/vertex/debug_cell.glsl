#version 130

in vec3 vertex_position;
in vec3 vertex_colour;

out vec3 frag_colour;

uniform mat4 u_mvp_matrix;


void main()
{
    frag_colour = vertex_colour;
    gl_Position = u_mvp_matrix * vec4(vertex_position, 1.0);
}
