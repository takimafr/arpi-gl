attribute vec3 a_position;
attribute vec3 a_normal;

uniform mat4 u_MVP;
uniform mat3 u_N;
uniform vec3 u_diffuse_color;

void main() {
  vec3 normal = a_normal * 0.03;
    vec4 pos = vec4(a_position + normal, 1.0);
    gl_Position = u_MVP * pos;
}