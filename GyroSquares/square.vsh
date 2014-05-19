
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;

uniform float inAlpha;

in vec4  inPosition;
in vec3  inNormal;
in vec4  inColor;

out vec3 position_eye, normal_eye;
out vec4 color;
out mat4 modelView;
out float alpha;

void main (void)
{
	// Transform the vertex by the model view projection matrix so
	// the polygon shows up in the right place
    position_eye = vec3(viewMatrix * modelMatrix * inPosition).xyz;
    normal_eye = vec3(viewMatrix * modelMatrix * vec4(inNormal, 0)).xyz;
    modelView = viewMatrix * modelMatrix;
    color = inColor;
    alpha = inAlpha;
    gl_Position	= projectionMatrix * vec4(position_eye, 1.0);
}