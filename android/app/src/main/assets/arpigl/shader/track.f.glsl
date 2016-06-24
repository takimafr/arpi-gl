#ifdef GL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#endif

uniform vec3 u_diffuse_color;

void main() {
    gl_FragColor = vec4(u_diffuse_color, 0.7);
}