#ifdef GL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#endif

varying vec4 v_lightIntensity;

void main() {
    gl_FragColor = v_lightIntensity;
}