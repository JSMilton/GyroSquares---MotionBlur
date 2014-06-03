
precision mediump float;

out vec4 outFragColor;
in vec2 st;

uniform sampler2D uColorTexture;
uniform sampler2D uVelocityTexture;
uniform sampler2D uDepthTexture;
uniform sampler2D uNeighbourMaxTexture;
uniform sampler2D uRandomTexture;

uniform       int uK;                    // In GatherShader
uniform       int uS;                    // In GatherShader
uniform     float uHalfExposure;         // In GatherShader
uniform     float uMaxSampleTapDistance; // In GatherShader

const vec2 VHALF =  vec2(0.5, 0.5);
const vec2 VONE  =  vec2(1.0, 1.0);
const vec2 VTWO  =  vec2(2.0, 2.0);

const float EPSILON1                 =   0.01;
const float EPSILON2                 =   0.001;
const float HALF_VELOCITY_CUTOFF     =   0.25;
const float SOFT_Z_EXTENT            =   0.1;
const float CYLINDER_CORNER_1        =   0.95;
const float CYLINDER_CORNER_2        =   1.05;
const float VARIANCE_THRESHOLD       =   1.5;
const float WEIGHT_CORRECTION_FACTOR =  60.0;

// Bias/scale helper functions
vec2 readBiasScale(vec2 v)
{
    return v * VTWO - VONE;
}

vec2 writeBiasScale(vec2 v)
{
    return (v + VONE) * VHALF;
}

// OTHER HELPER FUNCTIONS

float cone(float magDiff, float magV)
{
    return 1.0 - abs(magDiff) / magV;
}

float cylinder(float magDiff, float magV)
{
    return 1.0 - smoothstep(CYLINDER_CORNER_1 * magV,
                            CYLINDER_CORNER_2 * magV,
                            abs(magDiff));
}

float softDepthCompare(float za, float zb)
{
    return clamp( (1.0 - (za - zb) / SOFT_Z_EXTENT), 0.0, 1.0 );
}

float getDepth(ivec2 pos)
{
    // Negative since the paper says depth are negative
    return -(texelFetch(uDepthTexture, pos, 0).r);
}

float pseudoRandom(ivec2 pos)
{
    return texelFetch(uRandomTexture, pos % textureSize(uRandomTexture, 0), 0).r
    - 0.5;
}

void main(){
    // Some quick typecastings
    float fK = float(uK);
    float fS = float(uS);
    
    // X (position of current fragment)
    ivec2 ivX = ivec2(gl_FragCoord.xy);
    
    // C[X] (sample from color buffer at X)
    vec4 vCX = texelFetch(uColorTexture, ivX, 0);
    
    // VN = NeighborMax[X/k] (sample from NeighborMax buffer; holds dominant
    // half-velocity for the current fragment's neighborhood tile)
    vec2 vVN =
    readBiasScale(texelFetch(uNeighbourMaxTexture, (ivX / uK), 0).rg);
    float fLenVN = length(vVN);
    
    // Weighting, correcting and clamping half-velocity
    float fTempVN = fLenVN * uHalfExposure;
    bool bFlagVN = (fTempVN >= EPSILON1);
    fTempVN = clamp(fTempVN, 0.1, fK);
    
    //If the velocities are too short, we simply show the color texel and exit
    if(fTempVN <= (EPSILON2 + HALF_VELOCITY_CUTOFF))
    {
        outFragColor = vCX;
        return;
    }
    
    // Weighting, correcting and clamping half-velocity
    if(bFlagVN)
    {
        vVN *= (fTempVN / fLenVN);
        fLenVN = length(vVN);
    }
    
    // V[X] (sample from half-velocity buffer at X)
    vec2 vVX = readBiasScale(texelFetch(uVelocityTexture, ivX, 0).rg);
    float fLenVX = length(vVX);
    
    // Weighting, correcting and clamping half-velocity
    float fTempVX = fLenVX * uHalfExposure;
    bool bFlagVX = (fTempVX >= EPSILON1);
    fTempVX = clamp(fTempVX, 0.1, fK);
    if(bFlagVX)
    {
        vVX *= (fTempVX / fLenVX);
        fLenVX = length(vVX);
    }
    
    // Random value in [-0.5, 0.5]
    float fRandom = pseudoRandom(ivX);
    
    // Z[X] (depth value at X)
    float fZX = getDepth(ivX);
    
    // If V[X] (for current fragment) is too small,
    // then we use VN (for current tile)
    vec2 vCorrectedV = (fLenVX < VARIANCE_THRESHOLD) ?
    normalize(vVN) :
    normalize(vVX);
    
    // Weight value (suggested by the article authors' implementation)
    float fWeight = fS / WEIGHT_CORRECTION_FACTOR / fTempVX;
    
    // Cumulative sum (initialized to the current fragment,
    // since we skip it in the loop)
    vec3 vSum = vCX.rgb * fWeight;
    
    // Index for same fragment
    int iSelf = (uS - 1) / 2;
    
    // Iterate once for reconstruction sample tap
    for(int i = 0; i < uS; ++i)
    {
        // Skip the same fragment
        if(i == iSelf) { continue; }
        
        // T (distance between current fragment and sample tap)
        // NOTE: we are not sampling adjacent ones; we are extending our taps
        //       a little further
        float fT = mix(-uMaxSampleTapDistance, uMaxSampleTapDistance,
                       (float(i) + fRandom + 1.0) / (fS + 1.0));
        
        // The authors' implementation suggests alternating between the
        // corrected velocity and the neighborhood's
        vec2 vSwitchV = ((i & 1) == 1) ? vCorrectedV : vVN;
        
        // Y (position of current sample tap)
        ivec2 ivY = ivec2( ivX + ivec2(vSwitchV * fT + VHALF) );
        
        // V[Y] (sample from half-velocity buffer at Y)
        vec2 vVY = readBiasScale(texelFetch(uVelocityTexture, ivY, 0).rg);
        float fLenVY = length(vVY);
        
        // Weighting, correcting and clamping half-velocity
        float fTempVY = fLenVY * uHalfExposure;
        bool bFlagVY = (fTempVY >= EPSILON1);
        fTempVY = clamp(fTempVY, 0.1, fK);
        if(bFlagVY)
        {
            vVY *= (fTempVY / fLenVY);
            fLenVY = length(vVY);
        }
        
        // Z[Y] (depth value at Y)
        float fZY = getDepth(ivY);
        
        // alpha = foreground contribution + background contribution +
        //         blur of both foreground and background
        float alphaY = softDepthCompare(fZX, fZY) * cone(fT, fTempVY) +
        softDepthCompare(fZY, fZX) * cone(fT, fTempVX) +
        cylinder(fT, fTempVY) * cylinder(fT, fTempVX) * 2.0;
        
        // Applying to weight and weighted sum
        fWeight += alphaY;
        vSum += (alphaY * texelFetch(uColorTexture, ivY, 0).rgb);
    }
    
    outFragColor = vec4(vSum / fWeight, 1.0);
}