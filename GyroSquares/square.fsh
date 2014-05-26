
in vec3 position_eye, normal_eye;
in vec4 vPosition;
in vec4 vPreviousPosition;
in mat4 vViewMatrix;

layout (location = 0)out vec3 outColor;
layout (location = 1)out vec2 outVelocity;

uniform float testVelocity;

// fixed point light properties
vec3 light_position_world = vec3 (0.0, 0.0, 10.5);
vec3 Ls = vec3 (1.0, 1.0, 1.0); // white specular colour
vec3 Ld = vec3 (0.0, 0.5, 1.0); // dull white diffuse light colour
vec3 La = vec3 (0.1, 0.1, 0.1); // grey ambient colour

// surface reflectance
vec3 Ks = vec3 (0.5, 0.5, 0.5); // fully reflect specular light
vec3 Kd = vec3 (0.5, 0.5, 0.5); // orange diffuse surface reflectance
vec3 Ka = vec3 (1.0, 1.0, 1.0); // fully reflect ambient light
float specular_exponent = 0.01f; // specular 'power'

float light_power = 150;

void main (void)
{
    vec3 EyeDirection_eye = vec3(0,0,0) - position_eye;
    vec3 light_position_eye = vec3(vViewMatrix * vec4(light_position_world, 1.0)).xyz;
    vec3 light_direction_eye = light_position_eye + EyeDirection_eye;
    vec3 light_distance_eye = light_position_eye.xyz - position_eye.xyz;
    float LD = dot(light_distance_eye, light_distance_eye);
    vec3 n = normalize(normal_eye);
    vec3 l = normalize(light_direction_eye);
    float dot_prod = dot (n, l);
    dot_prod = max (dot_prod, 0.0);
    vec3 Id = Ld * Kd * dot_prod * light_power / LD; // final diffuse intensity
    
    // ambient intensity
    vec3 Ia = La * Kd * Ka * light_power * dot_prod / LD;
    
    vec3 E = normalize(EyeDirection_eye);
    vec3 R = reflect(-l, n);
    float cosAlpha = max(dot(E,R), 0);
    vec3 Is = Ls * Ks * light_power * pow(cosAlpha, 5) / LD; // final specular intensity
    
    // final colour
    outColor = vec3 (Is + Id + Ia).xyz;
    //outColor = vec3(1,1,1);
    vec3 a = (vPosition.xyz / vPosition.w);
    vec3 b = (vPreviousPosition.xyz / vPreviousPosition.w);
    outVelocity = vec2(a.x - b.x, a.y - b.y);
    
//    vec3 a = (vPosition.xyz) * 0.5 + 0.5;
//    vec3 b = (vPreviousPosition.xyz) * 0.5 + 0.5;
//    vec2 result = vec2(a.x - b.x, a.y - b.y);
//    velocity = pow(result, vec2(3.0,3.0));
}