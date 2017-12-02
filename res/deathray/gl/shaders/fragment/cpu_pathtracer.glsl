#version 130

in vec2 frag_uv;

uniform sampler2D u_texture;

out vec4 out_colour;

void main()
{
    out_colour = vec4(texture(u_texture, frag_uv).rgb, 1.0);
}
