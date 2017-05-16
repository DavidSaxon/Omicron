#version 130

in vec3 vertices;
in vec2 vertex_uv;
// layout(location = 0) in vec3 vertices;
// layout(location = 1) in vec2 vertex_uv;

out vec2 uv;

void main()
{
    gl_Position.xyz = vertices;
    gl_Position.w = 1.0;

    uv = vertex_uv;
}
