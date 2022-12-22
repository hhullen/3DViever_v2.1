attribute vec4 a_position;
attribute vec2 a_texcoord;
attribute vec3 a_normal;
uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;
uniform mat4 u_model_matrix;
varying vec4 v_position;
varying vec2 v_texcoord;
varying vec3 v_normal;

void main(void)
{
    mat4 mv_matrix = u_view_matrix * u_model_matrix;

    gl_Position = u_projection_matrix * mv_matrix * a_position;

    v_texcoord = a_texcoord;
    v_normal = normalize(vec3(mv_matrix * vec4(a_normal, 0.0)));
    v_position = mv_matrix * a_position;
}
