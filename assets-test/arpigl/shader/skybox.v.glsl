attribute vec3 a_position;

uniform mat4 u_MVP;

varying vec3 v_tex_coord;

void main() {
    vec4 pos = u_MVP * vec4(a_position, 1.0);
    gl_Position = pos.xyww;
    v_tex_coord = a_position;
}