
uniform mat4 modelViewProjectionMatrix;

#if __VERSION__ >= 140
in vec4  inPosition;
in vec4  inColor;
out vec4 varColor;
#else
#define lowp
attribute vec4 inPosition;
attribute vec4 inColor;
varying lowp vec4 varColor;
#endif

void main (void)
{
	// Transform the vertex by the model view projection matrix so
	// the polygon shows up in the right place
	gl_Position	= modelViewProjectionMatrix * inPosition;
    varColor = inColor;
}