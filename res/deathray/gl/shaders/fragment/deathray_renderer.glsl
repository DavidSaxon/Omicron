#version 130

in vec2 frag_uv;

out vec4 out_colour;

uniform sampler1D u_bvh;
uniform uint u_bvh_size;


void main()
{
    vec2 world_space = (frag_uv * 2.0) - 1.0;
    world_space *= 0.2;

    // check for cells in the bvh
    for(int i = 0; i < int(u_bvh_size); ++i)
    {
        float lookup = float(i) / float(u_bvh_size);
        vec3 bvh_pos = texture(u_bvh, lookup).xyz;

        vec2 bvh_min = bvh_pos.xy -= 0.01;
        vec2 bvh_max = bvh_pos.xy += 0.01;

        if(world_space.x >= bvh_min.x &&
           world_space.x <= bvh_max.x &&
           world_space.y >= bvh_min.y &&
           world_space.y <= bvh_max.y)
        {
            out_colour = vec4(0.0, 0.0, abs(bvh_pos.z * 10.0) + 0.2, 1.0);
            return;
        }
    }

    out_colour = vec4(0.7, 0.7, 0.7, 1.0);
    // out_colour = vec4(abs(world_space * 5.0), 0.0, 1.0);
}
