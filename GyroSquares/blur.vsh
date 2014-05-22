
// vertex positions input attribute
in vec2 inPosition;

// per-vertex texture coordinates input attribute
in vec2 inTexture;

// texture coordinates to be interpolated to fragment shaders
out vec2 st;
out vec4 viewRay;

uniform mat4 inverseProjectionMatrix;

void main (void) {
    
    viewRay = inverseProjectionMatrix * vec4(inPosition, 0.0, 1.0);
    // interpolate texture coordinates
    st = inTexture;
    // transform vertex position to clip space (camera view and perspective)
    gl_Position = vec4 (inPosition, 0.0, 1.0);
}