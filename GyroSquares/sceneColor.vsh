
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;

uniform mat4 uProjectionMatrix;
uniform mat4 uModelMatrix;
uniform mat4 uViewMatrix;

out vec3 vPosition_eye, vNormal_eye;
out mat4 vViewMatrix;

void main (void)
{
	// Transform the vertex by the model view projection matrix so
	// the polygon shows up in the right place
    vPosition_eye = vec3(uViewMatrix * uModelMatrix * vec4(inPosition, 1)).xyz;
    vNormal_eye = vec3(uViewMatrix * uModelMatrix * vec4(inNormal, 0)).xyz;
    gl_Position	= uProjectionMatrix * vec4(vPosition_eye, 1.0);
    vViewMatrix = uViewMatrix;
}