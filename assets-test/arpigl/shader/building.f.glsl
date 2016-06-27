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


vec3 scene_ambient = vec3(0.75, 0.75, 0.75);

void main() {

    Material material = Material(
      u_diffuse_color,
      u_diffuse_color,
      vec3(0.1, 0.1, 0.1),
      32.0
    );

//    LightSource light1 = LightSource(
//        vec4(-u_light0.position.x, -u_light0.position.y, -u_light0.position.z, 0.0),
//        u_light0.La / 2,
//        u_light0.Ld / 2,
//        u_light0.Ls / 2
//    );


    vec3 normal = normalize(v_normal);

    vec3 s0 = normalize(vec3(-u_light0.position));
    vec3 s1 = normalize(vec3(u_light0.position));
    //vec3 s1 = normalize(vec3(light1.position - v_eyePosition));

    vec3 sEye = normalize(vec3(-v_eyePosition));
    float sDotNEye = max(dot(sEye,normal), 0.0);

    vec3 ambient = scene_ambient * material.Ka;// + scene_ambient * material.Ka * sDotNEye;

    float sDotN0 = max(dot(s0,normal), 0.0);
    float sDotN1 = max(dot(s1,normal), 0.0);

    vec3 diffuse = u_light0.Ld * material.Kd * (sDotN0 + sDotN1);
    //diffuse = diffuse + light1.Ld * material.Kd * sDotN1;

    float distance = length(-v_eyePosition);
    float opacity = smoothstep(0.0, 30.0, distance);
    opacity = clamp(opacity, 0.8, 0.95);

    gl_FragColor = vec4(ambient + diffuse, opacity);
}

