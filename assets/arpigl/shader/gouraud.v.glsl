attribute vec3 a_position;
attribute vec3 a_normal;

varying vec4 v_lightIntensity;

uniform mat4 u_MVP, u_MV;
uniform mat3 u_N;


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
      vec3(0.3, 0.3, 0.3),
      vec3(1.0, 1.0, 1.0),
      5.0
    );


    vec4 position = vec4(a_position, 1.0);
    vec3 normal = normalize(u_N * a_normal);
    vec4 eyePosition = u_MV * position;

    vec3 s = normalize(vec3(light0.position - eyePosition));
    vec3 v = normalize(-eyePosition.xyz);
    vec3 r = reflect(-s, normal);

    vec3 ambient = vec3(0.3, 0.3, 0.3); //scene ambient

    float sDotN = max(dot(s,normal), 0.0);

    vec3 diffuse = light0.Ld * material.Kd * sDotN;

    vec3 specular = vec3(0.0);

    if(sDotN > 0.0) {
      specular = light0.Ls * material.Ks *
                 pow(max(dot(r,v),0.0), material.Shininess);
    }
    //specular = vec3(0.0);

    v_lightIntensity = vec4(ambient + diffuse + specular, 1.0);
    gl_Position = u_MVP * position;
}