
in vec4 vPosition;
in vec4 vPreviousPosition;

layout (location = 0) out vec2 outFragColor;

// UNIFORMS

// This value represents the (1/2 * exposure * framerate) part of the qX
// equation.
// NOTE ABOUT EXPOSURE: It represents the fraction of time that the exposure
//                      is open in the camera during a frame render. Larger
//                      values create more motion blur, smaller values create
//                      less blur. Arbitrarily defined by paper authors' in
//                      their source code as 75% and controlled by a slider
//                      in their demo.
uniform float ufHalfExposureXFramerate;
uniform float ufK;

// Constants
#define EPSILON 0.001
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

void main()
{
    vec2 vQX = ((vPosition.xy / vPosition.w) - (vPreviousPosition.xy/ vPreviousPosition.w)) * ufHalfExposureXFramerate;
    float fLenQX = length(vQX);
    
    float fWeight = max(0.5, min(fLenQX, ufK));
    fWeight /= ( fLenQX + EPSILON );
    
    vQX *= fWeight;
    
    outFragColor = writeBiasScale(vQX);
}