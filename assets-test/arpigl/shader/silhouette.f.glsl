#ifdef GL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#endif

uniform vec3 u_diffuse_color;
varying vec4 v_eyePosition;


void main() {
    gl_FragColor = vec4(u_diffuse_color, 1.0);
}