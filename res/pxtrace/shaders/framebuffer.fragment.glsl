#version 130

in vec2 uv;

out vec3 colour;

uniform sampler2D texture_sampler;

void main()
{
    vec3 c = texture(texture_sampler, uv).rgb;
    // c.g = 1.0;
    colour = c;
    // colour = texture(texture_sampler, uv).rgb;
    // colour = vec3(1, 0, 0);
}
