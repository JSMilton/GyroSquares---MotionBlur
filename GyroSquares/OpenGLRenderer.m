//
//  OpenGLRenderer.m
//  GyroSquares
//
//  Created by James Milton on 25/04/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#import "OpenGLRenderer.h"

#import <GLKit/GLKit.h>

#import "matrixUtil.h"
#import "sourceUtil.h"
#import "squareUtil.h"

#define GetGLError()									\
{														\
    GLenum err = glGetError();							\
    while (err != GL_NO_ERROR) {						\
        NSLog(@"GLError %s set in File:%s Line:%d\n",	\
                GetGLErrorString(err),					\
                __FILE__,								\
                __LINE__);								\
        err = glGetError();								\
    }													\
}

// Toggle this to disable vertex buffer objects
// (i.e. use client-side vertex array objects)
// This must be 1 if using the GL3 Core Profile on the Mac
#define USE_VERTEX_BUFFER_OBJECTS 1

// Toggle this to disable the rendering the reflection
// and setup of the GLSL progam, model and FBO used for
// the reflection.
#define RENDER_REFLECTION 1


// Indicies to which we will set vertex array attibutes
// See buildVAO and buildProgram
enum {
	POS_ATTRIB_IDX,
	NORMAL_ATTRIB_IDX,
    COLOR_ATTRIB_IDX
};

@implementation OpenGLRenderer
{
    SquareModel *m_squareModel;
    GLuint m_squarePrgName;
    GLint m_squareModelUniformIdx;
    GLint m_squareViewUniformIdx;
    GLint m_squareProjectionUniformIdx;
    GLuint m_squareVAOName;
    GLenum m_squarePrimType;
    GLenum m_squareElementType;
    GLuint m_squareNumElements;
    GLfloat m_squareAngle;
    
    SquareModel *m_squareModel2;
    GLuint m_squareVAOName2;
    GLenum m_squarePrimType2;
    GLenum m_squareElementType2;
    GLuint m_squareNumElements2;
    GLfloat m_squareAngle2;
    
    GLuint m_viewWidth;
    GLuint m_viewHeight;
    
    float mouseDeltaX;
    float mouseDeltaY;
}

- (void)resizeWithWidth:(GLuint)width AndHeight:(GLuint)height
{
	glViewport(0, 0, width, height);
    
	m_viewWidth = width;
	m_viewHeight = height;
}

- (void)moveCamera:(float)deltaX andDeltaY:(float)deltaY {
    mouseDeltaX += deltaX;
    mouseDeltaY += deltaY;
}

- (void)render {
    GLKMatrix4 model, view, projection;
    
//	GLfloat model[16];
//    GLfloat model2[16];
//    GLfloat view[16];
//	GLfloat projection[16];
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Use the program for rendering our square
	glUseProgram(m_squarePrgName);
    
    projection = GLKMatrix4MakePerspective(45.0f, (float)m_viewWidth / (float)m_viewHeight, 1.0, 100.0);
    view = GLKMatrix4MakeLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);
    view = GLKMatrix4RotateY(view, GLKMathDegreesToRadians(m_squareAngle));
    model = GLKMatrix4MakeTranslation(-5, 0, 0);
   // model = GLKMatrix4Scale(model, 0.5, 0.5, 0.5);
   // model = GLKMatrix4Rotate(model, GLKMathDegreesToRadians(m_squareAngle), 0, 1, 0);
	//model = GLKMatrix4Scale(model, 0.5, 0.5, 0.5);
	// Multiply the modelview and projection matrix and set it in the shader
	
	// Have our shader use the modelview projection matrix
	// that we calculated above
	glUniformMatrix4fv(m_squareModelUniformIdx, 1, GL_FALSE, &model.m00);
    glUniformMatrix4fv(m_squareViewUniformIdx, 1, GL_FALSE, &view.m00);
    glUniformMatrix4fv(m_squareProjectionUniformIdx, 1, GL_FALSE, &projection.m00);
	
	// Bind our vertex array object
	glBindVertexArray(m_squareVAOName);
	
	// Cull back faces now that we no longer render
	// with an inverted matrix
	//glCullFace(GL_BACK);
	
	// Draw our square
    glDrawElements(m_squarePrimType, m_squareNumElements, m_squareElementType, 0);
    
//    glBindVertexArray(m_squareVAOName2);
//    
//    mtxLoadScale(model2, 1, 1, 1);
//	mtxRotateApply(model2, m_squareAngle, 0, 1, 0);
//    //mtxRotateZApply(model2, m_squareAngle);
//    mtxScaleApply(model2, 2, 2, 0.2);
//	
//	// Multiply the modelview and projection matrix and set it in the shader
//    
//    glUniformMatrix4fv(m_squareModelUniformIdx, 1, GL_FALSE, model2);
//    glUniformMatrix4fv(m_squareViewUniformIdx, 1, GL_FALSE, view);
//    glUniformMatrix4fv(m_squareProjectionUniformIdx, 1, GL_FALSE, projection);
    
   // glDrawElements(m_squarePrimType2, m_squareNumElements2, m_squareElementType2, 0);

    // Update the angle so our square keeps spinning
	m_squareAngle++;
}

static GLsizei GetGLTypeSize(GLenum type)
{
	switch (type) {
		case GL_BYTE:
			return sizeof(GLbyte);
		case GL_UNSIGNED_BYTE:
			return sizeof(GLubyte);
		case GL_SHORT:
			return sizeof(GLshort);
		case GL_UNSIGNED_SHORT:
			return sizeof(GLushort);
		case GL_INT:
			return sizeof(GLint);
		case GL_UNSIGNED_INT:
			return sizeof(GLuint);
		case GL_FLOAT:
			return sizeof(GLfloat);
	}
    
	return 0;
}

- (GLuint)buildVAO:(SquareModel *)model {
    
    GLuint vaoName;
    glGenVertexArrays(1, &vaoName);
    glBindVertexArray(vaoName);
    
    GLuint posBufferName;
    
    // Create a vertex buffer object (VBO) to store positions
    glGenBuffers(1, &posBufferName);
    glBindBuffer(GL_ARRAY_BUFFER, posBufferName);
    
    // Allocate and load position data into the VBO
    glBufferData(GL_ARRAY_BUFFER, model->positionArraySize, model->positions, GL_STATIC_DRAW);
    
    // Enable the position attribute for this VAO
    glEnableVertexAttribArray(POS_ATTRIB_IDX);
    
    // Get the size of the position type so we can set the stride properly
    GLsizei posTypeSize = GetGLTypeSize(model->positionType);
    
    glVertexAttribPointer(POS_ATTRIB_IDX,		// What attibute index will this array feed in the vertex shader (see buildProgram)
                          model->positionSize,	// How many elements are there per position?
                          model->positionType,	// What is the type of this data?
                          GL_FALSE,				// Do we want to normalize this data (0-1 range for fixed-pont types)
                          model->positionSize*posTypeSize, // What is the stride (i.e. bytes between positions)?
                          0);	// What is the offset in the VBO to the position data?
    
    GLuint normalBufferName;
    // Create a vertex buffer object (VBO) to store positions
    glGenBuffers(1, &normalBufferName);
    glBindBuffer(GL_ARRAY_BUFFER, normalBufferName);
    
    // Allocate and load position data into the VBO
    glBufferData(GL_ARRAY_BUFFER, model->normalArraySize, model->normals, GL_STATIC_DRAW);
    
    // Enable the position attribute for this VAO
    glEnableVertexAttribArray(NORMAL_ATTRIB_IDX);
    
    // Get the size of the position type so we can set the stride properly
    posTypeSize = GetGLTypeSize(model->normalType);
    
    glVertexAttribPointer(NORMAL_ATTRIB_IDX,		// What attibute index will this array feed in the vertex shader (see buildProgram)
                          model->normalSize,	// How many elements are there per position?
                          model->normalType,	// What is the type of this data?
                          GL_FALSE,				// Do we want to normalize this data (0-1 range for fixed-pont types)
                          model->normalSize*posTypeSize, // What is the stride (i.e. bytes between positions)?
                          0);	// What is the offset in the VBO to the position data?
    
    GLuint colorBufferName;
    // Create a vertex buffer object (VBO) to store positions
    glGenBuffers(1, &colorBufferName);
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferName);
    
    // Allocate and load position data into the VBO
    glBufferData(GL_ARRAY_BUFFER, model->colorArraySize, model->colors, GL_STATIC_DRAW);
    
    // Enable the position attribute for this VAO
    glEnableVertexAttribArray(COLOR_ATTRIB_IDX);
    
    // Get the size of the position type so we can set the stride properly
    posTypeSize = GetGLTypeSize(model->colorType);
    
    glVertexAttribPointer(COLOR_ATTRIB_IDX,		// What attibute index will this array feed in the vertex shader (see buildProgram)
                          model->colorSize,	// How many elements are there per position?
                          model->colorType,	// What is the type of this data?
                          GL_FALSE,				// Do we want to colorize this data (0-1 range for fixed-pont types)
                          model->colorSize*posTypeSize, // What is the stride (i.e. bytes between positions)?
                          0);	// What is the offset in the VBO to the position data?

    GLuint elementBufferName;
    // Create a VBO to vertex array elements
    // This also attaches the element array buffer to the VAO
    glGenBuffers(1, &elementBufferName);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferName);
    
    // Allocate and load vertex array element data into VBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->elementArraySize, model->elements, GL_STATIC_DRAW);
    
    return vaoName;
}

-(void)destroyVAO:(GLuint) vaoName
{
	GLuint index;
	GLuint bufName;
	
	// Bind the VAO so we can get data from it
	glBindVertexArray(vaoName);
	
	// For every possible attribute set in the VAO
	for(index = 0; index < 16; index++)
	{
		// Get the VBO set for that attibute
		glGetVertexAttribiv(index , GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING, (GLint*)&bufName);
		
		// If there was a VBO set...
		if(bufName)
		{
			//...delete the VBO
			glDeleteBuffers(1, &bufName);
		}
	}
	
	// Get any element array VBO set in the VAO
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, (GLint*)&bufName);
	
	// If there was a element array VBO set in the VAO
	if(bufName)
	{
		//...delete the VBO
		glDeleteBuffers(1, &bufName);
	}
	
	// Finally, delete the VAO
	glDeleteVertexArrays(1, &vaoName);
	
	GetGLError();
}

- (id)initWithDefaultFBO: (GLuint) defaultFBOName
{
	if((self = [super init]))
	{
		NSLog(@"%s %s", glGetString(GL_RENDERER), glGetString(GL_VERSION));
		
		////////////////////////////////////////////////////
		// Build all of our and setup initial state here  //
		// Don't wait until our real time run loop begins //
		////////////////////////////////////////////////////
		
		m_defaultFBOName = defaultFBOName;
		m_viewWidth = 100;
		m_viewHeight = 100;
		m_squareAngle = 0;
        m_squareAngle2 = 0;
        
		//////////////////////////////
		// Create Model             //
		//////////////////////////////
        
        m_squareModel = loadCube();
		
		// Build Vertex Buffer Objects (VBOs) and Vertex Array Object (VAOs) with our model data: Mine will be a cube, not an awesome monster :(
		m_squareVAOName = [self buildVAO:m_squareModel];
		
		// Cache the number of element and primType to use later in our glDrawElements calls
		m_squareNumElements = m_squareModel->numElements;
		m_squarePrimType = m_squareModel->primType;
		m_squareElementType = m_squareModel->elementType;
        

        //If we're using VBOs we can destroy all this memory since buffers are
        // loaded into GL and we've saved anything else we need
        destroySquareModel(m_squareModel);
        m_squareModel = NULL;
        
        m_squareModel2 = loadHollowCuboid();
		
		// Build Vertex Buffer Objects (VBOs) and Vertex Array Object (VAOs) with our model data: Mine will be a cube, not an awesome monster :(
		m_squareVAOName2 = [self buildVAO:m_squareModel2];
		
		// Cache the number of element and primType to use later in our glDrawElements calls
		m_squareNumElements2 = m_squareModel2->numElements;
		m_squarePrimType2 = m_squareModel2->primType;
		m_squareElementType2 = m_squareModel2->elementType;
        
        
        //If we're using VBOs we can destroy all this memory since buffers are
        // loaded into GL and we've saved anything else we need
        destroySquareModel(m_squareModel2);
        m_squareModel2 = NULL;
		
		////////////////////////////////////////////////////
		// Load and Setup shaders for square rendering //
		////////////////////////////////////////////////////
		
        
        NSString *filePathName = @"";
		demoSource *vtxSource = NULL;
		demoSource *frgSource = NULL;
		
		filePathName = [[NSBundle mainBundle] pathForResource:@"square" ofType:@"vsh"];
		vtxSource = srcLoadSource([filePathName cStringUsingEncoding:NSASCIIStringEncoding]);
		
		filePathName = [[NSBundle mainBundle] pathForResource:@"square" ofType:@"fsh"];
		frgSource = srcLoadSource([filePathName cStringUsingEncoding:NSASCIIStringEncoding]);
		
		// Build Program
		m_squarePrgName = [self buildProgramWithVertexSource:vtxSource
											 withFragmentSource:frgSource
													 withNormal:NO
												   withTexcoord:NO];
		
		srcDestroySource(vtxSource);
		srcDestroySource(frgSource);
		
        m_squareViewUniformIdx = glGetUniformLocation(m_squarePrgName, "viewMatrix");
		m_squareModelUniformIdx = glGetUniformLocation(m_squarePrgName, "modelMatrix");
        m_squareProjectionUniformIdx = glGetUniformLocation(m_squarePrgName, "projectionMatrix");
		
		if(m_squareModelUniformIdx < 0)
		{
			NSLog(@"No modelViewProjectionMatrix in square shader");
		}
		
		////////////////////////////////////////////////
		// Set up OpenGL state that will never change //
		////////////////////////////////////////////////
		
		// Depth test will always be enabled
		glEnable(GL_DEPTH_TEST);
        
		// We will always cull back faces for better performance
	//	glEnable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		
		// Always use this clear color
		glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
		
		// Draw our scene once without presenting the rendered image.
		//   This is done in order to pre-warm OpenGL
		// We don't need to present the buffer since we don't actually want the
		//   user to see this, we're only drawing as a pre-warm stage
		[self render];
		
		// Reset the m_squareAngle which is incremented in render
		m_squareAngle = 0;
		
		// Check for errors to make sure all of our setup went ok
		GetGLError();
	}
	
	return self;
}

-(GLuint) buildProgramWithVertexSource:(demoSource*)vertexSource
					withFragmentSource:(demoSource*)fragmentSource
							withNormal:(BOOL)hasNormal
						  withTexcoord:(BOOL)hasTexcoord
{
	GLuint prgName;
	
	GLint logLength, status;
	
	// String to pass to glShaderSource
	GLchar* sourceString = NULL;
	
	// Determine if GLSL version 140 is supported by this context.
	//  We'll use this info to generate a GLSL shader source string
	//  with the proper version preprocessor string prepended
	float  glLanguageVersion;
	sscanf((char *)glGetString(GL_SHADING_LANGUAGE_VERSION), "%f", &glLanguageVersion);
	
	// GL_SHADING_LANGUAGE_VERSION returns the version standard version form
	//  with decimals, but the GLSL version preprocessor directive simply
	//  uses integers (thus 1.10 should 110 and 1.40 should be 140, etc.)
	//  We multiply the floating point number by 100 to get a proper
	//  number for the GLSL preprocessor directive
	GLuint version = 100 * glLanguageVersion;
	
	// Get the size of the version preprocessor string info so we know
	//  how much memory to allocate for our sourceString
	const GLsizei versionStringSize = sizeof("#version 123\n");
	
	// Create a program object
	prgName = glCreateProgram();
	
	// Indicate the attribute indicies on which vertex arrays will be
	//  set with glVertexAttribPointer
	//  See buildVAO to see where vertex arrays are actually set
	glBindAttribLocation(prgName, POS_ATTRIB_IDX, "inPosition");
    glBindAttribLocation(prgName, NORMAL_ATTRIB_IDX, "inNormal");
    glBindAttribLocation(prgName, COLOR_ATTRIB_IDX, "inColor");
	
	//////////////////////////////////////
	// Specify and compile VertexShader //
	//////////////////////////////////////
	
	// Allocate memory for the source string including the version preprocessor information
	sourceString = malloc(vertexSource->byteSize + versionStringSize);
	
	// Prepend our vertex shader source string with the supported GLSL version so
	//  the shader will work on ES, Legacy, and OpenGL 3.2 Core Profile contexts
	sprintf(sourceString, "#version %d\n%s", version, vertexSource->string);
    
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar **)&(sourceString), NULL);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);
	
	if (logLength > 0)
	{
		GLchar *log = (GLchar*) malloc(logLength);
		glGetShaderInfoLog(vertexShader, logLength, &logLength, log);
		NSLog(@"Vtx Shader compile log:%s\n", log);
		free(log);
	}
	
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == 0)
	{
		NSLog(@"Failed to compile vtx shader:\n%s\n", sourceString);
		return 0;
	}
	
	free(sourceString);
	sourceString = NULL;
	
	// Attach the vertex shader to our program
	glAttachShader(prgName, vertexShader);
	
	// Delete the vertex shader since it is now attached
	// to the program, which will retain a reference to it
	glDeleteShader(vertexShader);
	
	/////////////////////////////////////////
	// Specify and compile Fragment Shader //
	/////////////////////////////////////////
	
	// Allocate memory for the source string including the version preprocessor	 information
	sourceString = malloc(fragmentSource->byteSize + versionStringSize);
	
	// Prepend our fragment shader source string with the supported GLSL version so
	//  the shader will work on ES, Legacy, and OpenGL 3.2 Core Profile contexts
	sprintf(sourceString, "#version %d\n%s", version, fragmentSource->string);
	
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragShader, 1, (const GLchar **)&(sourceString), NULL);
	glCompileShader(fragShader);
	glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		GLchar *log = (GLchar*)malloc(logLength);
		glGetShaderInfoLog(fragShader, logLength, &logLength, log);
		NSLog(@"Frag Shader compile log:\n%s\n", log);
		free(log);
	}
	
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &status);
	if (status == 0)
	{
		NSLog(@"Failed to compile frag shader:\n%s\n", sourceString);
		return 0;
	}
	
	free(sourceString);
	sourceString = NULL;
	
	// Attach the fragment shader to our program
	glAttachShader(prgName, fragShader);
	
	// Delete the fragment shader since it is now attached
	// to the program, which will retain a reference to it
	glDeleteShader(fragShader);
	
	//////////////////////
	// Link the program //
	//////////////////////
	
	glLinkProgram(prgName);
	glGetProgramiv(prgName, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		GLchar *log = (GLchar*)malloc(logLength);
		glGetProgramInfoLog(prgName, logLength, &logLength, log);
		NSLog(@"Program link log:\n%s\n", log);
		free(log);
	}
	
	glGetProgramiv(prgName, GL_LINK_STATUS, &status);
	if (status == 0)
	{
		NSLog(@"Failed to link program");
		return 0;
	}
	
	glValidateProgram(prgName);
	glGetProgramiv(prgName, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0)
	{
		GLchar *log = (GLchar*)malloc(logLength);
		glGetProgramInfoLog(prgName, logLength, &logLength, log);
		NSLog(@"Program validate log:\n%s\n", log);
		free(log);
	}
	
	glGetProgramiv(prgName, GL_VALIDATE_STATUS, &status);
	if (status == 0)
	{
		NSLog(@"Failed to validate program");
		return 0;
	}
	
	glUseProgram(prgName);
	
	GetGLError();
	
	return prgName;
}

- (void) dealloc
{
	// Cleanup all OpenGL objects and
	[self destroyVAO:m_squareVAOName];
	glDeleteProgram(m_squarePrgName);
	destroySquareModel(m_squareModel);
}

@end
