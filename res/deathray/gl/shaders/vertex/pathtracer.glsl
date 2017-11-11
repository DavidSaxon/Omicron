#version 130

in vec3 vertex_position;
in vec2 vertex_image_plane;

out vec2 frag_image_plane;

void main()
{
    frag_image_plane = vertex_image_plane;
    gl_Position = vec4(vertex_position, 1.0);
}
