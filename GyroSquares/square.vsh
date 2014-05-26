
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 previousModelViewProjectionMatrix;

layout (location = 0) in vec4  inPosition;
layout (location = 1) in vec3  inNormal;

out vec3 position_eye, normal_eye;
out mat4 vViewMatrix;

out vec4 vPosition;
out vec4 vPreviousPosition;

void main (void)
{
	// Transform the vertex by the model view projection matrix so
	// the polygon shows up in the right place
    position_eye = vec3(viewMatrix * modelMatrix * inPosition).xyz;
    normal_eye = vec3(viewMatrix * modelMatrix * vec4(inNormal, 0)).xyz;
    vPosition = projectionMatrix * vec4(position_eye, 1.0);
    vPreviousPosition = previousModelViewProjectionMatrix * inPosition;
    gl_Position	= vPosition;
    vViewMatrix = viewMatrix;
}