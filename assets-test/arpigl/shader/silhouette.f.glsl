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
    float distance = length(v_eyePosition);
    float distance_opacity = smoothstep(0.0, 150.0, distance);
    distance_opacity = clamp(distance_opacity, 0.0, 0.40);

    float opacity = 0.5 + distance_opacity;
    gl_FragColor = vec4(u_diffuse_color, opacity);
}