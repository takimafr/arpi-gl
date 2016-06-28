attribute vec3 a_position;
attribute vec3 a_normal;

uniform mat4 u_MVP;
uniform mat4 u_MV;
uniform mat3 u_N;
uniform vec3 u_diffuse_color;
varying vec4 v_eyePosition;


void main() {
    vec4 position = vec4(a_position,  1.0);
    v_eyePosition = u_MV * position;
    vec3 normal = a_normal * (0.045);
    position = position + vec4(normal, 0.0);
    gl_Position = u_MVP * position;
}