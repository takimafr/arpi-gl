#ifdef GL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#endif

uniform samplerCube u_cube_map;
varying vec3 v_tex_coord;

void main() {
    vec4 texel = textureCube(u_cube_map, v_tex_coord);
    texel.a = 1.0;
    gl_FragColor = texel;
}