#ifdef GL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#endif

uniform vec3 u_diffuse_color;

struct Material
{
  vec3 Ka;
  vec3 Kd;
  vec3 Ks;
  float Shininess;
};


void main() {

    Material material = Material(
      vec3(0.1, 0.1, 0.1),
      vec3(0.2, 0.4, 0.2),
      vec3(1.0, 1.0, 1.0),
      5.0
    );

    gl_FragColor = vec4(u_diffuse_color, 0.7);
}