
out vec4 outFragColor;
in vec2 st;

uniform sampler2D uColorTexture;
uniform sampler2D uVelocityTexture;
uniform sampler2D uDepthTexture;
uniform sampler2D uNeighbourMaxTexture;

const vec2 VHALF =  vec2(0.5, 0.5);
const vec2 VONE  =  vec2(1.0, 1.0);
const vec2 VTWO  =  vec2(2.0, 2.0);

// Bias/scale helper functions
vec2 readBiasScale(vec2 v)
{
    return v * VTWO - VONE;
}

vec2 writeBiasScale(vec2 v)
{
    return (v + VONE) * VHALF;
}

void main(){
    vec4 velocity = vec4(readBiasScale(texture(uVelocityTexture, st).xy), 0.5, 1);
    vec4 color = vec4(texture(uColorTexture, st).xyz, 1);
    vec4 neighbour = vec4(texture(uNeighbourMaxTexture, st).xy, 0.5, 1);
    outFragColor = neighbour;
}