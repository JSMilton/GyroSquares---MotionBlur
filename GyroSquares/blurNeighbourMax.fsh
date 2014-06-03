
// OUTPUT
out vec2 outFragColor;

// UNIFORMS
uniform sampler2D uInputTexture; // In BaseFullscreenShader

// CONSTANTS
const vec2 VHALF =  vec2(0.5, 0.5);
const vec2 VONE  =  vec2(1.0, 1.0);
const vec2 VTWO  =  vec2(2.0, 2.0);

const ivec2 IVZERO = ivec2(0, 0);
const ivec2 IVONE  = ivec2(1, 1);

const vec2 GRAY = vec2(0.5, 0.5);

// Bias/scale helper functions
vec2 readBiasScale(vec2 v)
{
    return v * VTWO - VONE;
}

vec2 writeBiasScale(vec2 v)
{
    return (v + VONE) * VHALF;
}

float compareWithNeighbor(ivec2 tileCorner, int s, int t, float maxMagnitudeSquared)
{
    ivec2 ivTexSizeMinusOne = textureSize(uInputTexture, 0) - IVONE;
    ivec2 ivOffset = ivec2(s, t);
    vec2 vVelocity = readBiasScale(texelFetch(uInputTexture,
                                              clamp(tileCorner + ivOffset, IVZERO, ivTexSizeMinusOne), 0).rg);
    float fMagnitudeSquared = dot(vVelocity, vVelocity);
    
    if(maxMagnitudeSquared < fMagnitudeSquared)
    {
        float fDisplacement = abs(float(ivOffset.x)) + abs(float(ivOffset.y));
        vec2 vOrientation = sign(vec2(ivOffset) * vVelocity);
        float fDistance = float(vOrientation.x + vOrientation.y);
        
        if(abs(fDistance) == fDisplacement)
        {
            outFragColor = writeBiasScale(vVelocity);
            maxMagnitudeSquared = fMagnitudeSquared;
        }
    }
    return maxMagnitudeSquared;
}

void main()
{
    outFragColor = GRAY;
    
    ivec2 ivTileCorner = ivec2(gl_FragCoord.xy);
    float fMaxMagnitudeSquared = 0.0;
    
    fMaxMagnitudeSquared =
    compareWithNeighbor(ivTileCorner, -1, -1, fMaxMagnitudeSquared);
    fMaxMagnitudeSquared =
    compareWithNeighbor(ivTileCorner,  0, -1, fMaxMagnitudeSquared);
    fMaxMagnitudeSquared =
    compareWithNeighbor(ivTileCorner,  1, -1, fMaxMagnitudeSquared);
    fMaxMagnitudeSquared =
    compareWithNeighbor(ivTileCorner, -1,  0, fMaxMagnitudeSquared);
    fMaxMagnitudeSquared =
    compareWithNeighbor(ivTileCorner,  0,  0, fMaxMagnitudeSquared);
    fMaxMagnitudeSquared =
    compareWithNeighbor(ivTileCorner,  1,  0, fMaxMagnitudeSquared);
    fMaxMagnitudeSquared =
    compareWithNeighbor(ivTileCorner, -1,  1, fMaxMagnitudeSquared);
    fMaxMagnitudeSquared =
    compareWithNeighbor(ivTileCorner,  0,  1, fMaxMagnitudeSquared);
    fMaxMagnitudeSquared =
    compareWithNeighbor(ivTileCorner,  1,  1, fMaxMagnitudeSquared);
}