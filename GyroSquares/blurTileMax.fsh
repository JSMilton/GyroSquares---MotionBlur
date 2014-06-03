out vec2 outFragColor;

// UNIFORMS
uniform sampler2D uInputTexture; // In BaseFullscreenShader
uniform int uK;              // In TileMaxShader

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

void main()
{
    ivec2 ivTileCorner = ivec2(gl_FragCoord.xy) * uK;
    ivec2 ivTexSizeMinusOne = textureSize(uInputTexture, 0) - IVONE;
    
    outFragColor = GRAY;
    
    float fMaxMagnitudeSquared = 0.0;
    
    for(int s = 0; s < uK; ++s)
    {
        for(int t = 0; t < uK; ++t)
        {
            vec2 vVelocity = readBiasScale(texelFetch(uInputTexture,
                                                      clamp(ivTileCorner + ivec2(s, t), IVZERO, ivTexSizeMinusOne),
                                                      0).rg);
            
            float fMagnitudeSquared = dot(vVelocity, vVelocity);
            if(fMaxMagnitudeSquared < fMagnitudeSquared)
            {
                outFragColor = writeBiasScale(vVelocity);
                fMaxMagnitudeSquared = fMagnitudeSquared;
            }
        }
    }
}