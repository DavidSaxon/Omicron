#version 130

in vec3 vertex_position;

uniform vec3 u_bound_min;
uniform vec3 u_bound_max;
uniform mat4 u_mvp_matrix;

void main()
{
    vec3 bound_pos = vertex_position;
    // x
    if(bound_pos.x < 0.0)
    {
        bound_pos.x = u_bound_min.x;
    }
    else if(bound_pos.x > 0.0)
    {
        bound_pos.x = u_bound_max.x;
    }
    // y
    if(bound_pos.y < 0.0)
    {
        bound_pos.y = u_bound_min.y;
    }
    else if(bound_pos.y > 0.0)
    {
        bound_pos.y = u_bound_max.y;
    }
    // z
    if(bound_pos.z < 0.0)
    {
        bound_pos.z = u_bound_min.z;
    }
    else if(bound_pos.z > 0.0)
    {
        bound_pos.z = u_bound_max.z;
    }

    gl_Position = u_mvp_matrix * vec4(bound_pos, 1.0);
}
