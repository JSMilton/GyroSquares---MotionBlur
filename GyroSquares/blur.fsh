
// texture coordinates from vertex shaders
in vec2 st;

// texture sampler
uniform sampler2D velocityTexure;
uniform sampler2D colorTexture;

out vec4 frag_colour;

void main (void) {

    vec2 velocityVector = texture(velocityTexure, st).xy / 6.0;
    vec4 color = vec4(0.0);
    vec2 texCoord = st;
    
    texCoord -= velocityVector;
    color += texture(colorTexture, texCoord) * 0.5;
    texCoord -= velocityVector;
    color += texture(colorTexture, texCoord) * 0.4;
    texCoord -= velocityVector;
    color += texture(colorTexture, texCoord) * 0.3;
    texCoord -= velocityVector;
    color += texture(colorTexture, texCoord) * 0.2;
    
    frag_colour = color;
}