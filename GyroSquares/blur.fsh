
// texture coordinates from vertex shaders
in vec2 st;
in vec4 viewRay;

// texture sampler
uniform sampler2D tex;
uniform sampler2D velocityTex;
uniform sampler2D depthTex;
uniform mat4 inverseModelViewMatrix;
uniform mat4 previousModelViewProjectionMatrix;

// output fragment colour RGBA
out vec4 frag_colour;

void main (void) {
    
//    vec4 current = normalize(viewRay) * texture(depthTex, st).r;
//    current = inverseModelViewMatrix * current;
//    
//    vec4 previous = previousModelViewProjectionMatrix * current;
//    previous.xyz /= previous.w;
//    previous.xy = previous.xy * 0.5 + 0.5;
//    
//    vec2 blurVec = (previous.xy - st) * 0.005;
//    
//    // perform blur:
//    vec4 result = texture(tex, st);
//    float nSamples = 20.0f;
//    for (int i = 1; i < nSamples; ++i) {
//        // get offset in range [-0.5, 0.5]:
//        vec2 offset = (blurVec * (float(i) / float(nSamples - 1) - 0.5));
//        
//        // sample & add to result:
//        result += texture(tex, st + offset);
//    }
//    
//    float f = 100.0;
//    float n = 0.1;
//    float z = (2 * n) / (f + n - texture(depthTex, st).x * (f - n));
//    
//    float d = texture(depthTex, st).r;
//    frag_colour = result /= float(nSamples);
    
    vec2 texelSize = 1.0 / vec2(textureSize(tex, 0));
    vec2 vel = vec2(texture(velocityTex, st).rg);// * 2.0 - 1.0;
    //vel *= 10;
   // vel = pow(vel, vec2(1.0/3.0,1.0/3.0));
    //vel = vel * 2.0 - 1.0;
    float speed = length(vel / texelSize);
    float nSamples = 20;// clamp(int(speed), 1, 20);
   // vel *= .01;
    vec4 result = texture(tex, st);
    float f = vel.r;
    float d = vel.g;

//    for (int i = 1; i < nSamples; ++i) {
//        // get offset in range [-0.5, 0.5]:
//        vec2 offset = (vel * (float(i) / float(nSamples - 1) - 0.5));
//
//        // sample & add to result
//        result += texture(tex, st + offset);
//    }
    frag_colour = vec4(result.r,d,result.z,1);// result;// result;// result;// /= float(nSamples);
}