#ifdef GL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#endif

uniform sampler2D u_diffuse_map;
varying vec2 v_uv;

void main() {
    vec4 texel = texture2D(u_diffuse_map, v_uv);
	gl_FragColor = texel;
}