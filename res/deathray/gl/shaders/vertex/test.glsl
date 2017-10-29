#version 130

in vec3 in_position;

uniform mat4 u_vp_matrix;

out vec3 colour;

void main()
{
    colour = vec3(
        abs(in_position.x) * 8.0,
        abs(in_position.y) * 8.0,
        abs(in_position.z) * 8.0
    );
    vec4 position = vec4(in_position, 1.0);
    gl_Position = u_vp_matrix * position;
}
