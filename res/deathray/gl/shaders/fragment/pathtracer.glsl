#version 130

in vec2 frag_image_plane;

uniform sampler2D u_ibl_texture;
uniform sampler2D u_nmap_texture;
uniform sampler2D u_nmap2_texture;
uniform sampler2D u_test_texture;

uniform mat4 u_camera_postion;

out vec4 out_colour;


bool ray_sphere_intersection_geometric(
        vec3 ray_origin,
        vec3 ray_direction,
        vec3 sphere_centre,
        float sphere_radius,
        inout float t0,
        inout float t1)
{
    // L the distance from the origin of the ray to the centre of the sphere
    vec3 L = sphere_centre - ray_origin;
    // project L onto the direction of the ray
    float tca = dot(L, ray_direction);
    // is the sphere behind the origin?
    if(tca < 0.0)
    {
        return false;
    }

    float radius2 = sphere_radius * sphere_radius;
    float d2 = dot(L, L) - (tca * tca);

    if(d2 > radius2)
    {
        return false;
    }

    float thc = sqrt(radius2 - d2);
    t0 = tca - thc;
    t1 = tca + thc;

    return true;
}

bool ray_plane_intersection(
        vec3 ray_origin,
        vec3 ray_direction,
        vec3 plane_position,
        vec3 plane_normal,
        inout float t)
{
    // assuming vectors are all normalized
    float denom = dot(plane_normal, ray_direction);
    if(denom > 0.000001)
    {
        vec3 p0l0 = plane_position - ray_origin;
        t = dot(p0l0, plane_normal) / denom;
        return t >= 0.0;
    }
    return false;
}

//------------------------------------------------------------------------------
bool check_sphere_1(
        vec3 ray_origin,
        vec3 ray_direction,
        inout vec3 out_ray_origin,
        inout vec3 out_ray_direction,
        inout vec3 contrib)
{
    vec3 position = vec3(0.7, 0.0, 1.0);
    float radius = 0.65;
    float t0 = 0.0;
    float t1 = 0.0;
    if(ray_sphere_intersection_geometric(
        ray_origin,
        ray_direction,
        position,
        radius,
        t0,
        t1
    ))
    {
        vec3 near_intersect = ray_origin + (t0 * ray_direction);
        vec3 normal = normalize(near_intersect - position);

        out_ray_origin = near_intersect;
        out_ray_direction = reflect(ray_direction, normal);
        contrib = vec3(0.96, 0.96, 0.96);
        return true;
    }
    return false;
}

bool check_sphere_2(
        vec3 ray_origin,
        vec3 ray_direction,
        inout vec3 out_ray_origin,
        inout vec3 out_ray_direction,
        inout vec3 contrib)
{
    vec3 position = vec3(-0.7, 0.0, 1.0);
    float radius = 0.6;
    float t0 = 0.0;
    float t1 = 0.0;
    if(ray_sphere_intersection_geometric(
        ray_origin,
        ray_direction,
        position,
        radius,
        t0,
        t1
    ))
    {
        vec3 near_intersect = ray_origin + (t0 * ray_direction);
        vec3 normal = normalize(near_intersect - position);

        out_ray_origin = near_intersect;
        out_ray_direction = reflect(ray_direction, normal);
        contrib = vec3(0.96, 0.4, 0.4);
        return true;
    }
    return false;
}

bool check_sphere_3(
        vec3 ray_origin,
        vec3 ray_direction,
        inout vec3 out_ray_origin,
        inout vec3 out_ray_direction,
        inout vec3 contrib)
{
    vec3 position = vec3(-0.7, 0.3, -1.0);
    float radius = 0.65;
    float t0 = 0.0;
    float t1 = 0.0;
    if(ray_sphere_intersection_geometric(
        ray_origin,
        ray_direction,
        position,
        radius,
        t0,
        t1
    ))
    {
        vec3 near_intersect = ray_origin + (t0 * ray_direction);

        vec3 normal = normalize(near_intersect - position);
        vec2 ray2_n = normalize(normal.xz);
        vec2 uv = vec2(
            abs(-1.0 * atan(ray2_n.y - -1.0, ray2_n.x - 0.0)) / 3.1415926,
            (normal.y + 1.0) / 2.0
        );



        normal += texture(u_nmap2_texture, uv).rgb / 2.5;
        normal = normalize(normal);

        out_ray_origin = near_intersect;
        out_ray_direction = reflect(ray_direction, normal);
        contrib = vec3(0.4, 0.96, 0.4);
        // contrib = texture(u_nmap_texture, uv).rgb;
        return true;
    }
    return false;
}

bool check_sphere_4(
        vec3 ray_origin,
        vec3 ray_direction,
        inout vec3 out_ray_origin,
        inout vec3 out_ray_direction,
        inout vec3 contrib)
{
    vec3 position = vec3(0.3, -0.3, -1.5);
    float radius = 0.2;
    float t0 = 0.0;
    float t1 = 0.0;
    if(ray_sphere_intersection_geometric(
        ray_origin,
        ray_direction,
        position,
        radius,
        t0,
        t1
    ))
    {
        vec3 near_intersect = ray_origin + (t0 * ray_direction);
        vec3 normal = normalize(near_intersect - position);

        out_ray_origin = near_intersect;
        out_ray_direction = reflect(ray_direction, normal);
        contrib = vec3(0.96, 0.96, 0.6);
        return true;
    }
    return false;
}

bool check_plane_1(
        vec3 ray_origin,
        vec3 ray_direction,
        inout vec3 out_ray_origin,
        inout vec3 out_ray_direction,
        inout vec3 contrib)
{
    // vec3 position = vec3(0.0, -0.65, 0.0);
    vec3 position = vec3(0.0, 0, -0.01);
    // TODO: should be y up
    // vec3 normal = vec3(0.0, -1.0, 0.0);
    vec3 normal = vec3(0.0, 0.0, -1.0);
    float t = 0.0;

    if(ray_plane_intersection(
        ray_origin,
        ray_direction,
        position,
        normal,
        t
    ))
    {
        vec3 intersect = ray_origin + (ray_direction * t);

        normal += vec3(
            texture(u_nmap_texture, (intersect.xy + 2.0) / 4.0).rg / 16.0,
            0.0
        );
        normal = normalize(normal);

        // apply ripples
        if(false)
        {
            float d = distance(intersect, position) * 15.0;
            normal.x = sin(d) * 0.02;
            normal.y = cos(d) * 0.02;
            normal = normalize(normal);
        }

        if(abs(intersect.x) <= 2.0 && abs(intersect.y) <= 2.0)
        {
            out_ray_origin = intersect;
            out_ray_direction = reflect(ray_direction, normal);
            contrib = vec3(0.9, 0.95, 1.0);
            // contrib = texture(u_nmap_texture, (intersect.xy + 2.0) / 4.0).rgb;
            return true;
        }
    }

    // backface
    t = 0.0;
    position.z *= 1.01;
    normal = vec3(0.0, 0.0, 1.0);
    if(ray_plane_intersection(
        ray_origin,
        ray_direction,
        position,
        normal,
        t
    ))
    {
        // return true;
        vec3 intersect = ray_origin + (ray_direction * t);

        // apply ripples
        if(true)
        {
            float d = distance(intersect, position) * 15.0;
            normal.x = sin(d) * 0.02;
            normal.y = cos(d) * 0.02;
            normal = normalize(normal);
        }

        if(abs(intersect.x) <= 2.0 && abs(intersect.y) <= 2.0)
        {
            out_ray_origin = intersect;
            out_ray_direction = reflect(ray_direction, normal);
            contrib = vec3(0.7, 0.7, 0.7);
            contrib +=
                texture(u_test_texture, (intersect.xy + 2.0) / 4.0).rgb;
            return true;
        }
    }

    return false;
}

bool check_disk_1(
        vec3 ray_origin,
        vec3 ray_direction,
        inout vec3 out_ray_origin,
        inout vec3 out_ray_direction,
        inout vec3 contrib)
{
    // vec3 position = vec3(0.0, -0.65, 0.0);
    vec3 position = vec3(0.0, 0, -2.0);
    // TODO: should be y up
    // vec3 normal = vec3(0.0, -1.0, 0.0);
    vec3 normal = vec3(0.0, 0.0, 1.0);
    float t = 0.0;

    if(ray_plane_intersection(
        ray_origin,
        ray_direction,
        position,
        normal,
        t
    ))
    {
        vec3 intersect = ray_origin + (ray_direction * t);

        // apply ripples
        if(true)
        {
            float d = distance(intersect, position);
            normal.x = sin(d) * 1.4;
            normal.y = cos(d) * 1.4;
            normal = normalize(normal);
        }

        if(distance(intersect, position) < 1.0)
        {
            out_ray_origin = intersect - (position * 0.01);
            out_ray_direction = refract(ray_direction, -normal, 0.66);
            contrib = vec3(1.0, 1.0, 1.0);
            return true;
        }
    }

    return false;
}

bool environment_trace(
        inout vec3 ray_origin,
        inout vec3 ray_direction,
        inout vec3 colour)
{
    bool contact = false;
    float d = 1000000.0;
    vec3 t_ray_origin = ray_origin;
    vec3 t_ray_direction = ray_direction;
    vec3 t_colour = colour;

    vec3 sphere_ray_origin = vec3(0.0, 0.0, 0.0);
    vec3 sphere_ray_direction = vec3(0.0, 0.0, 0.0);
    vec3 sphere_contrib = vec3(1.0, 1.0, 1.0);
    bool sphere_hit = check_sphere_1(
        ray_origin,
        ray_direction,
        sphere_ray_origin,
        sphere_ray_direction,
        sphere_contrib
    );
    if(sphere_hit)
    {
        t_ray_origin = sphere_ray_origin;
        t_ray_direction = sphere_ray_direction;
        t_colour = sphere_contrib;
        d = distance(sphere_ray_origin, ray_origin);
        contact = true;
    }

    vec3 sphere2_ray_origin = vec3(0.0, 0.0, 0.0);
    vec3 sphere2_ray_direction = vec3(0.0, 0.0, 0.0);
    vec3 sphere2_contrib = vec3(1.0, 1.0, 1.0);
    bool sphere2_hit = check_sphere_2(
        ray_origin,
        ray_direction,
        sphere2_ray_origin,
        sphere2_ray_direction,
        sphere2_contrib
    );
    if(sphere2_hit)
    {
        float s2_d = distance(sphere2_ray_origin, ray_origin);
        if(contact)
        {
            if(s2_d > d)
            {
                sphere2_hit = false;
            }
        }
        if(sphere2_hit)
        {
            t_ray_origin = sphere2_ray_origin;
            t_ray_direction = sphere2_ray_direction;
            t_colour = sphere2_contrib;
            d = s2_d;
            contact = true;
        }
    }

    vec3 sphere3_ray_origin = vec3(0.0, 0.0, 0.0);
    vec3 sphere3_ray_direction = vec3(0.0, 0.0, 0.0);
    vec3 sphere3_contrib = vec3(1.0, 1.0, 1.0);
    bool sphere3_hit = check_sphere_3(
        ray_origin,
        ray_direction,
        sphere3_ray_origin,
        sphere3_ray_direction,
        sphere3_contrib
    );
    if(sphere3_hit)
    {
        float s3_d = distance(sphere3_ray_origin, ray_origin);
        if(contact)
        {
            if(s3_d > d)
            {
                sphere3_hit = false;
            }
        }
        if(sphere3_hit)
        {
            t_ray_origin = sphere3_ray_origin;
            t_ray_direction = sphere3_ray_direction;
            t_colour = sphere3_contrib;
            d = s3_d;
            contact = true;
        }
    }

    vec3 sphere4_ray_origin = vec3(0.0, 0.0, 0.0);
    vec3 sphere4_ray_direction = vec3(0.0, 0.0, 0.0);
    vec3 sphere4_contrib = vec3(1.0, 1.0, 1.0);
    bool sphere4_hit = check_sphere_4(
        ray_origin,
        ray_direction,
        sphere4_ray_origin,
        sphere4_ray_direction,
        sphere4_contrib
    );
    if(sphere4_hit)
    {
        float s4_d = distance(sphere4_ray_origin, ray_origin);
        if(contact)
        {
            if(s4_d > d)
            {
                sphere4_hit = false;
            }
        }
        if(sphere4_hit)
        {
            t_ray_origin = sphere4_ray_origin;
            t_ray_direction = sphere4_ray_direction;
            t_colour = sphere4_contrib;
            d = s4_d;
            contact = true;
        }
    }

    vec3 plane_ray_origin = vec3(0.0, 0.0, 0.0);
    vec3 plane_ray_direction = vec3(0.0, 0.0, 0.0);
    vec3 plane_contrib = vec3(1.0, 1.0, 1.0);
    bool plane_hit = check_plane_1(
        ray_origin,
        ray_direction,
        plane_ray_origin,
        plane_ray_direction,
        plane_contrib
    );
    if(plane_hit)
    {
        float p_d = distance(plane_ray_origin, ray_origin);
        if(contact)
        {
            if(p_d > d)
            {
                plane_hit = false;
            }
        }
        if(plane_hit)
        {
            t_ray_origin = plane_ray_origin;
            t_ray_direction = plane_ray_direction;
            t_colour = plane_contrib;
            d = p_d;
            contact = true;
        }
    }

    // vec3 disk_ray_origin = vec3(0.0, 0.0, 0.0);
    // vec3 disk_ray_direction = vec3(0.0, 0.0, 0.0);
    // vec3 disk_contrib = vec3(1.0, 1.0, 1.0);
    // bool disk_hit = check_disk_1(
    //     ray_origin,
    //     ray_direction,
    //     disk_ray_origin,
    //     disk_ray_direction,
    //     disk_contrib
    // );
    // if(disk_hit)
    // {
    //     float d_d = distance(disk_ray_origin, ray_origin);
    //     if(contact)
    //     {
    //         if(d_d > d)
    //         {
    //             disk_hit = false;
    //         }
    //     }
    //     if(disk_hit)
    //     {
    //         t_ray_origin = disk_ray_origin;
    //         t_ray_direction = disk_ray_direction;
    //         t_colour = disk_contrib;
    //         d = d_d;
    //         contact = true;
    //     }
    // }

    if(contact)
    {
        ray_origin = t_ray_origin;
        ray_direction = t_ray_direction;
        colour *= t_colour;
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------

void main()
{
    // TODO: uniforms
    // float u_half_fov = 0.785398;
    float u_half_fov = 0.785398;

    // TODO: could be uniform
    // direction of the camera
    vec3 camera_direction = vec3(0.0, 0.0, -1.0);

    // TODO: need to take into account aspect ratio
    // build the origin of the ray
    vec3 ray_origin = vec3(frag_image_plane.x, frag_image_plane.y, 0.0);
    // vec3 ray_origin = vec3(0.0, 0.0, 0.0);
    // build the direction vector of the ray
    vec3 ray_direction = vec3(
        sin(frag_image_plane.x * u_half_fov),
        sin(frag_image_plane.y * u_half_fov),
        -1.0
    );
    ray_direction = normalize(ray_direction);

    vec3 o_ray = ray_direction;

    // apply camera
    // camera_direction =
        // normalize((u_camera_postion * vec4(camera_direction, 0.0)).xyz);
    ray_origin = (u_camera_postion * vec4(ray_origin, 1.0)).xyz;
    ray_direction = normalize((u_camera_postion * vec4(ray_direction, 0.0)).xyz);

    //--------------------------------------------------------------------------
    // int collision = 0;
    // vec3 sphere_centre = vec3(0.0, 0.0, 0.0);
    // float t0 = 0.0;
    // float t1 = 0.0;
    // if(ray_sphere_intersection_geometric(
    //     ray_origin,
    //     ray_direction,
    //     sphere_centre,
    //     1.0,
    //     t0,
    //     t1
    // ))
    // {
    //     vec3 near_intersect = ray_origin + (t0 * ray_direction);
    //     // near_intersect
    //     // TODO:

    //     // vec3 normal =   normalize(near_intersect);
    //     // vec3 normal = vec3(-1.0, 0.0, 0.0);
    //     vec3 normal = normalize(near_intersect - sphere_centre);

    //     collision = 1;
    //     ray_origin = near_intersect;
    //     ray_direction = reflect(ray_direction, normal);
    //     // ray_direction = normal;
    //     // out_colour = vec4(abs(ray_direction), 1.0);
    //     // return;

    //     // out_colour = vec4(abs(near_intersect), 1.0);
    //     // return;
    // }
    //--------------------------------------------------------------------------


    //--------------------------------------------------------------------------
    // else if(ray_plane_intersection(
    //     ray_origin,
    //     ray_direction,
    //     vec3(0.0, -1.0,  0.0),
    //     vec3(0.0, -1.0, 0.0),
    //     t0
    // ))
    // {
    //     // out_colour = vec4(1.0, 0.0, 0.0, 1.0);
    //     // return;

    //     vec3 intersect = ray_origin + (ray_direction * t0);
    //     ray_origin = intersect;
    //     if(abs(intersect.x) <= 2.0 && abs(intersect.z) <= 2.0)
    //     {
    //         collision = 2;
    //         float d = distance(intersect, vec3(0.0, -1.0, 0.0)) * 15.0;
    //         ray_direction = reflect(
    //             ray_direction,
    //             normalize(vec3(sin(d) * 0.02, -1.0, cos(d) * 0.02))
    //         );
    //     }
    // }
    //--------------------------------------------------------------------------

    vec3 colour = vec3(1.0, 1.0, 1.0);

    for(uint i = 0U; i < 5U; ++i)
    {
        if(!environment_trace(ray_origin, ray_direction, colour))
        {
            break;
        }
    }

    // // TODO: needs to be looped for bounces
    // vec3 sphere_ray_origin = vec3(0.0, 0.0, 0.0);
    // vec3 sphere_ray_direction = vec3(0.0, 0.0, 0.0);
    // vec3 sphere_contrib = vec3(1.0, 1.0, 1.0);
    // if(check_sphere_1(
    //     ray_origin,
    //     ray_direction,
    //     sphere_ray_origin,
    //     sphere_ray_direction,
    //     sphere_contrib
    // ))
    // {
    //     ray_origin = sphere_ray_origin;
    //     ray_direction = sphere_ray_direction;
    //     colour *= sphere_contrib;
    // }

    // vec3 plane_ray_origin = vec3(0.0, 0.0, 0.0);
    // vec3 plane_ray_direction = vec3(0.0, 0.0, 0.0);
    // vec3 plane_contrib = vec3(1.0, 1.0, 1.0);
    // if(check_plane_1(
    //     ray_origin,
    //     ray_direction,
    //     plane_ray_origin,
    //     plane_ray_direction,
    //     plane_contrib
    // ))
    // {
    //     ray_origin = plane_ray_origin;
    //     ray_direction = plane_ray_direction;
    //     colour *= plane_contrib;
    // }


    // calculate the uvs of the IBL
    // vec3 env_dir = ray_origin + ray_direction;
    vec3 env_dir = ray_direction;
    vec2 ray2_n = normalize(env_dir.xz);
    vec2 ibl_uv = vec2(
        abs(-1.0 * atan(ray2_n.y - -1.0, ray2_n.x - 0.0)) / 3.1415926,
        (env_dir.y + 1.0) / 2.0
    );

    out_colour = vec4(texture(u_ibl_texture, ibl_uv).rgb * colour, 1.0);
    // out_colour = vec4(ibl_uv, 0.0, 1.0);

    // if(collision == 1)
    // {
    //     out_colour.r *= 0.95;
    //     out_colour.g *= 0.95;
    //     out_colour.b *= 0.95;
    // }
    // else if(collision == 2)
    // {
    //     out_colour.x *= 0.5;
    //     out_colour.y *= 0.8;
    //     out_colour.z *= 1.0;
    // }
}
