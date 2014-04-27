
#if __VERSION__ >= 140
in vec4      varColor;
out vec4     fragColor;
#else
#define lowp
varying lowp vec4 varColor;
#endif

// fixed point light properties
vec3 light_position_world = vec3 (10.0, 10.0, 10.0);
vec3 Ls = vec3 (1.0, 1.0, 1.0); // white specular colour
vec3 Ld = vec3 (0.7, 0.7, 0.7); // dull white diffuse light colour
vec3 La = vec3 (0.2, 0.2, 0.2); // grey ambient colour

// surface reflectance
vec3 Ks = vec3 (1.0, 1.0, 1.0); // fully reflect specular light
vec3 Kd = vec3 (1.0, 0.5, 0.0); // orange diffuse surface reflectance
vec3 Ka = vec3 (1.0, 1.0, 1.0); // fully reflect ambient light
float specular_exponent = 100.0; // specular 'power'

void main (void)
{
#if __VERSION__ >= 140
	fragColor = varColor;
#else
    gl_FragColor = varColor;
#endif
}