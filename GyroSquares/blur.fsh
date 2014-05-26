
// texture coordinates from vertex shaders
in vec2 st;

// texture sampler
uniform sampler2D velocityTexure;
uniform sampler2D colorTexture;

out vec4 frag_colour;

void main (void) {

//    vec2 velocityVector = texture(velocityTexure, st).xy / 10.0;
//    vec4 color = vec4(0.0);
//    vec2 texCoord = st;
//    
//    texCoord -= velocityVector;
//    color += texture(colorTexture, texCoord) * 0.5;
//    texCoord -= velocityVector;
//    color += texture(colorTexture, texCoord) * 0.4;
//    texCoord -= velocityVector;
//    color += texture(colorTexture, texCoord) * 0.3;
//    texCoord -= velocityVector;
//    color += texture(colorTexture, texCoord) * 0.2;
//    
//    frag_colour = color;
    
    vec2 vel = texture(velocityTexure, st).xy;
    float nSamples = 10;// clamp(int(speed), 1, 20);
    vec4 result = texture(colorTexture, st);
    
    for (int i = 1; i < nSamples; ++i) {
        // get offset in range [-0.5, 0.5]:
        vec2 offset = (vel * (float(i) / float(nSamples - 1) - 0.5));

        // sample & add to result
        result += texture(colorTexture, st + offset);
    }
    frag_colour = result /= float(nSamples);
}