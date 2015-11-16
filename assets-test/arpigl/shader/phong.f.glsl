#ifdef GL_ES
#ifdef GL_FRAGMENT_PRECISION_HIGH
precision highp float;
#else
precision mediump float;
#endif
#endif

struct LightSource
{
  vec4 position;
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

uniform mat4 u_MV;
uniform vec3 u_diffuse_color;
uniform LightSource u_light0;


varying vec3 v_normal;
varying vec4 v_eyePosition;


vec3 scene_ambient = vec3(0.2, 0.2, 0.2);

void main() {

    Material material = Material(
      u_diffuse_color,
      u_diffuse_color,
      vec3(0.5, 0.5, 0.5),
      31.0
    );

    vec3 normal = normalize(v_normal);

    vec3 s = normalize(vec3(u_light0.position - v_eyePosition));
    vec3 v = normalize(-v_eyePosition.xyz);
    vec3 r = reflect(-s, normal);


    vec3 sEye = normalize(vec3(-v_eyePosition));
    float sDotNEye = max(dot(sEye,normal), 0.0);

    vec3 ambient = scene_ambient * material.Ka * sDotNEye * 3.0;

    float sDotN = max(dot(s,normal), 0.0);

    vec3 diffuse = u_light0.Ld * material.Kd * sDotN;

    vec3 specular = vec3(0.0);

    if (sDotN > 0.0) {
        specular = u_light0.Ls * material.Ks *
                   pow(max(dot(r,v),0.0), material.Shininess);
    }

    gl_FragColor = vec4(ambient + diffuse + specular, 1.0);
}

