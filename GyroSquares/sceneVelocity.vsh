
layout (location = 0) in vec3 inPosition;

out vec4 vPosition;
out vec4 vPreviousPosition;

uniform mat4 uProjectionMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uPreviousModelMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uPreviousViewMatrix;

void main()
{
    vPosition = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4(inPosition, 1);
    vPreviousPosition = uProjectionMatrix * uPreviousViewMatrix * uPreviousModelMatrix * vec4(inPosition, 1);
    
    gl_Position = vPosition;
}
