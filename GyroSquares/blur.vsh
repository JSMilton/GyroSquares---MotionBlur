
// vertex positions input attribute
layout (location = 0) in vec3 inPosition;

// per-vertex texture coordinates input attribute
layout (location = 2) in vec2 inTexture;

// texture coordinates to be interpolated to fragment shaders
out vec2 st;

void main (void) {
    // interpolate texture coordinates
    st = inTexture;
    // transform vertex position to clip space (camera view and perspective)
    gl_Position = vec4(inPosition, 1.0);
}