attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_uv;

uniform mat4 u_MVP, u_MV;
uniform mat3 u_N;
uniform vec3 u_diffuse_color;

varying vec3 v_normal;
varying vec2 v_uv;
varying vec4 v_eyePosition;


void main() {

    vec4 position = vec4(a_position, 1.0);
    v_eyePosition = u_MV * position;
    v_normal = normalize(u_N * a_normal);
    v_uv = a_uv;
    gl_Position = u_MVP * position;
}