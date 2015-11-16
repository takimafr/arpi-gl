#ifdef GL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#endif

uniform mat4 u_MVP, u_MV;
uniform mat3 u_N;

varying vec3 v_normal;
varying vec4 v_eyePosition;

struct LightSource
{
  vec4 position; //in eye coords
  vec3 La;
  vec3 Ld;  //diffuse light intensity
  vec3 Ls;
};


struct Material
{
  vec3 Ka;
  vec3 Kd;
  vec3 Ks;
  float Shininess;
};


void main() {

    LightSource light0 = LightSource(
      vec4(1.0, 2.0, 1.0, 0.0), //in eye coords
      vec3(1.0, 1.0, 1.0),
      vec3(1.0, 1.0, 1.0),
      vec3(1.0, 1.0, 1.0)
    );

    Material material = Material(
      vec3(0.1, 0.1, 0.1),
      vec3(0.2, 0.4, 0.2),
      vec3(1.0, 1.0, 1.0),
      5.0
    );

    vec3 normal = normalize(v_normal);

    vec3 s = normalize(vec3(light0.position - v_eyePosition));
    vec3 v = normalize(-v_eyePosition.xyz);
    vec3 r = reflect(-s, normal);

    vec3 ambient = vec3(0.3, 0.3, 0.3); //scene ambient

    float sDotN = max(dot(s,normal), 0.0);

    vec3 diffuse = light0.Ld * material.Kd * sDotN;

    vec3 specular = vec3(0.0);

    gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
}