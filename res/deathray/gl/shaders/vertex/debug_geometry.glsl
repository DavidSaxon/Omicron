#version 130

in vec3 vertex_position;

uniform mat4 u_mvp_matrix;
uniform vec3 u_size;

out vec3 frag_colour;

void main()
{
    frag_colour = vec3(abs(vertex_position) / u_size);
    gl_Position = u_mvp_matrix * vec4(vertex_position, 1.0);
}
