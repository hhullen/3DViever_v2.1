uniform sampler2D u_texture;
uniform vec4 u_light_position;
uniform float u_light_power;
uniform vec4 u_light_color;
uniform vec4 u_polygon_color;
uniform vec4 u_shadow_color;
uniform int u_shade_mode;
uniform int u_is_textured;
varying vec4 v_position;
varying vec2 v_texcoord;
varying vec3 v_normal;

void main(void)
{
    vec4 result_color = u_shadow_color;
    vec4 eye_position = vec4(0.0, 0.0, 0.0, 1.0);

    vec4 diffuse_mat_color = u_polygon_color;
    if (u_is_textured == 1) {
        diffuse_mat_color = texture2D(u_texture, v_texcoord);
    }

    vec3 eye_vector = normalize(v_position.xyz - eye_position.xyz);
    vec3 light_vector = normalize(v_position.xyz - u_light_position.xyz);
    vec3 reflect_light = normalize(reflect(light_vector, v_normal));
    float len = length(v_position.xyz - eye_position.xyz);
    float specular_factor = 200.0;
    float ambient_factor = 0.1;

    if (u_shade_mode == 0) {
        vec4 diffuse_color = diffuse_mat_color  * u_light_power * max(0.0, dot(v_normal, -light_vector)) * ambient_factor;
        result_color += diffuse_color * u_light_color;
    } else if (u_shade_mode == 1) {
        vec4 diffuse_color = diffuse_mat_color  * u_light_power * max(0.0, dot(v_normal, -light_vector)) / (1.0 + 0.25 * pow(len, 2.0));
        result_color += diffuse_color * u_light_color;
        vec4 ambient_color = ambient_factor * diffuse_color;
        result_color += ambient_color;
        vec4 highlight_power = vec4(1.0, 1.0, 1.0, 1.0);
        vec4 specular_color = highlight_power * u_light_power * pow(max(0.0, dot(reflect_light, -eye_vector)), specular_factor) / (1.0 + 0.25 * pow(len, 2.0));
        result_color += specular_color;
    } else {
        result_color += u_polygon_color;
    }

     gl_FragColor = result_color;

}
