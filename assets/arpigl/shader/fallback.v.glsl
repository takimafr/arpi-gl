attribute vec3 a_position;

uniform mat4 u_MVP, u_MV;
uniform mat3 u_N;

attribute vec2 a_uv;
varying vec2 v_uv;


void main() {

    vec4 position = vec4(a_position, 1.0);
    gl_Position = u_MVP * position;
    
    //it's up to fragment shader to deal with texcoord
    v_uv = a_uv;
}