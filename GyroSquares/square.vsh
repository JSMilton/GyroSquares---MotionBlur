
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

in vec4  inPosition;
in vec4  inNormal;
in vec4  inColor;
out vec3 position_eye, normal_eye;
out vec4 color;
out mat4 modelView;

void main (void)
{
	// Transform the vertex by the m-odel view projection matrix so
	// the polygon shows up in the right place
    position_eye = vec3(viewMatrix * modelMatrix * inPosition);
    normal_eye = vec3(viewMatrix * modelMatrix * inNormal);
    modelView = viewMatrix * modelMatrix;
    color = inColor;
    gl_Position	= projectionMatrix * vec4(position_eye, 1.0);
}