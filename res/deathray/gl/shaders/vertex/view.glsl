#version 130

in vec3 vertex_position;
in vec2 vertex_uv;

out vec2 frag_uv;

void main()
{
    frag_uv = vertex_uv;
    gl_Position = vec4(vertex_position, 1.0);
}
