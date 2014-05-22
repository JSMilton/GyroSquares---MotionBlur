//
//  OpenGLRenderer.m
//  GyroSquares
//
//  Created by James Milton on 25/04/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#import "OpenGLRenderer.h"

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
    COLOR_ATTRIB_IDX,
    TEXTURE_ATTRIB_IDX
};

const int maxFrames = 50;

typedef struct {
    float alpha;
    GLKMatrix4 modelMatrix;
} Frame;

typedef struct {
    Frame frames[maxFrames];
    int index;
} FrameControl;

@implementation OpenGLRenderer
{
    SquareModel *m_squareModel;
    GLuint m_squarePrgName;
    GLint m_squareModelUniformIdx;
    GLint m_squareViewUniformIdx;
    GLint m_squareProjectionUniformIdx;
    GLint m_alphaUniformIndex;
    GLuint m_squareVAOName;
    GLenum m_squarePrimType;
    GLenum m_squareElementType;
    GLuint m_squareNumElements;
    
    SquareModel *m_squareModel2;
    GLuint m_squareVAOName2;
    GLenum m_squarePrimType2;
    GLenum m_squareElementType2;
    GLuint m_squareNumElements2;
    GLfloat m_squareAngle2;
    
    SquareModel *m_screenQuad;
    GLuint m_screenQuadVAO;
    GLuint m_screenQuadElementType;
    GLuint m_screenQuadPrimType;
    GLuint m_screenQuadNumElements;
    
    GLuint m_blurProgram;
    
    GLuint m_colorTexture;
    GLuint m_velocityTexture;
    GLuint m_depthTexture;
    GLuint m_frameBuffer;
    GLint  m_colorTextureUniformIdx;
    GLint  m_depthTextureUniformIdx;
    GLint  m_velocityTextureUniformIdx;
    GLint  m_inverseModelViewMatrixIdx;
    GLint  m_inverseProjectionMatrix;
    GLint  m_previousModelViewProjectionUniformIdx;
    
    GLuint m_viewWidth;
    GLuint m_viewHeight;
    
    GLKVector3 cubeVelocityVector;
    GLKVector3 cubeRotationVector;
    GLKVector3 cubeOldRotationVector;
    
    GLKVector3 frameVelocityVector;
    GLKVector3 frameRotationVector;
    GLKVector3 frameOldRotationVector;
    
    GLKMatrix4 previousModelViewProjectionMatrix;
    GLKMatrix4 previousModelViewProjectionMatrix2;
    GLint m_previousMVPMatrixFirstPass;
    
    FrameControl frameControl;
}

- (void)resizeWithWidth:(GLuint)width AndHeight:(GLuint)height andIsLive:(BOOL)isLive
{
	glViewport(0, 0, width, height);
    
	m_viewWidth = width;
	m_viewHeight = height;
    
    if (!isLive){
        m_frameBuffer = [self buildFrameBuffer];
    }
}

- (void)moveCamera:(GLKVector3)vector {
    
}

int velMod = 500;
- (void)moveInnerCube:(GLKVector3)vector {
    cubeVelocityVector.x += vector.x / velMod;
    cubeVelocityVector.y += vector.y / velMod;
    cubeVelocityVector.z += vector.z / velMod;
}

- (void)moveOuterFrame:(GLKVector3)vector {
    frameVelocityVector.x += vector.x / velMod;
    frameVelocityVector.y += vector.y / velMod;
    frameVelocityVector.z += vector.z / velMod;
}

- (void)render {
    glEnable(GL_DEPTH_TEST);
    GLKMatrix4 model, model2, view, projection;
    
    glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer);
    glClearDepth(100.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	// Use the program for rendering our square
	glUseProgram(m_squarePrgName);
    
    projection = GLKMatrix4MakePerspective(GLKMathDegreesToRadians(45.0f), (float)m_viewWidth / (float)m_viewHeight, 0.1, 100.0);
    view = GLKMatrix4MakeLookAt(0, 0, 20, 0, 0, 0, 0, 1, 0);
    model = GLKMatrix4MakeTranslation(0, 0, -2);
    model = GLKMatrix4RotateX(model, GLKMathDegreesToRadians(cubeRotationVector.y));
    model = GLKMatrix4RotateY(model, GLKMathDegreesToRadians(cubeRotationVector.x));
	
	// Have our shader use the modelview projection matrix
	// that we calculated above
	glUniformMatrix4fv(m_squareModelUniformIdx, 1, GL_FALSE, &model.m00);
    glUniformMatrix4fv(m_squareViewUniformIdx, 1, GL_FALSE, &view.m00);
    glUniformMatrix4fv(m_squareProjectionUniformIdx, 1, GL_FALSE, &projection.m00);
    glUniformMatrix4fv(m_previousMVPMatrixFirstPass, 1, GL_FALSE, &previousModelViewProjectionMatrix.m00);
	
	// Bind our vertex array object
	glBindVertexArray(m_squareVAOName);
	
	// Cull back faces now that we no longer render
	// with an inverted matrix
	//glCullFace(GL_BACK);
	
	// Draw our square
    glDrawElements(m_squarePrimType, m_squareNumElements, m_squareElementType, 0);
    
    glBindVertexArray(m_squareVAOName2);
    
    model2 = GLKMatrix4MakeTranslation(0, 0, -2);
    model2 = GLKMatrix4RotateX(model2, GLKMathDegreesToRadians(frameRotationVector.x));
    model2 = GLKMatrix4RotateY(model2, GLKMathDegreesToRadians(frameRotationVector.y));
    model2 = GLKMatrix4Scale(model2, 3, 3, 0.25);
	
	// Multiply the modelview and projection matrix and set it in the shader
    
    glUniformMatrix4fv(m_squareModelUniformIdx, 1, GL_FALSE, &model2.m00);
    glUniformMatrix4fv(m_squareViewUniformIdx, 1, GL_FALSE, &view.m00);
    glUniformMatrix4fv(m_squareProjectionUniformIdx, 1, GL_FALSE, &projection.m00);
    glUniformMatrix4fv(m_previousMVPMatrixFirstPass, 1, GL_FALSE, &previousModelViewProjectionMatrix2.m00);
    
    glDrawElements(m_squarePrimType2, m_squareNumElements2, m_squareElementType2, 0);

    cubeRotationVector.x += cubeVelocityVector.x;
    cubeRotationVector.y += cubeVelocityVector.y;
    cubeRotationVector.z += cubeVelocityVector.z;
    cubeVelocityVector.x *= 0.99;
    cubeVelocityVector.y *= 0.99;
    cubeVelocityVector.z *= 0.99;
    
    frameRotationVector.x += frameVelocityVector.x;
    frameRotationVector.y += frameVelocityVector.y;
    frameRotationVector.z += frameVelocityVector.z;
    frameVelocityVector.x *= 0.99;
    frameVelocityVector.y *= 0.99;
    frameVelocityVector.z *= 0.99;
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClearDepth(1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(m_blurProgram);
    
    GLKMatrix4 modelViewMatrix = GLKMatrix4Multiply(view, model2);
    
    bool isInvertible = false;
    GLKMatrix4 inverseModelViewMatrix = GLKMatrix4Invert(modelViewMatrix, &isInvertible);
    if (!isInvertible) NSLog(@"failed to invert model view matrix");
    GLKMatrix4 modelViewProjectionMatrix = GLKMatrix4Multiply(GLKMatrix4Multiply(projection, view), model);
    GLKMatrix4 modelViewProjectionMatrix2 = GLKMatrix4Multiply(GLKMatrix4Multiply(projection, view), model2);
    GLKMatrix4 inverseProjectionMatrix = GLKMatrix4Invert(projection, &isInvertible);
    if (!isInvertible) NSLog(@"failed to invert projection matrix");
    
    glUniformMatrix4fv(m_previousModelViewProjectionUniformIdx, 1, GL_FALSE, &previousModelViewProjectionMatrix.m00);
    glUniformMatrix4fv(m_inverseModelViewMatrixIdx, 1, GL_FALSE, &inverseModelViewMatrix.m00);
    glUniformMatrix4fv(m_inverseProjectionMatrix, 1, GL_FALSE, &inverseProjectionMatrix.m00);
    
    glBindVertexArray(m_screenQuadVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_colorTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_velocityTexture);
    glUniform1i(m_colorTextureUniformIdx, 0);
    glUniform1i(m_depthTextureUniformIdx, 1);
    glUniform1i(m_velocityTextureUniformIdx, 2);
    glDrawArrays(m_screenQuadPrimType, 0, m_screenQuadNumElements);
    
    glActiveTexture(GL_TEXTURE0);
    
    previousModelViewProjectionMatrix = modelViewProjectionMatrix;
    previousModelViewProjectionMatrix2 = modelViewProjectionMatrix2;
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
    
    if (model->normalSize > 0){
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
    }
    
    if (model->colorSize > 0){
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
    }
    
    if (model->textureSize > 0){
        GLuint textureBufferName;
        // Create a vertex buffer object (VBO) to store positions
        glGenBuffers(1, &textureBufferName);
        glBindBuffer(GL_ARRAY_BUFFER, textureBufferName);
        
        // Allocate and load position data into the VBO
        glBufferData(GL_ARRAY_BUFFER, model->texureUVArraySize, model->textureUV, GL_STATIC_DRAW);
        
        // Enable the position attribute for this VAO
        glEnableVertexAttribArray(TEXTURE_ATTRIB_IDX);
        
        // Get the size of the position type so we can set the stride properly
        posTypeSize = GetGLTypeSize(model->textureType);
        
        glVertexAttribPointer(TEXTURE_ATTRIB_IDX,		// What attibute index will this array feed in the vertex shader (see buildProgram)
                              model->textureSize,	// How many elements are there per position?
                              model->textureType,	// What is the type of this data?
                              GL_FALSE,				// Do we want to colorize this data (0-1 range for fixed-pont types)
                              model->textureSize*posTypeSize, // What is the stride (i.e. bytes between positions)?
                              0);	// What is the offset in the VBO to the position data?
    }
    
    if (model->elementArraySize > 0){
        GLuint elementBufferName;
        // Create a VBO to vertex array elements
        // This also attaches the element array buffer to the VAO
        glGenBuffers(1, &elementBufferName);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferName);
        
        // Allocate and load vertex array element data into VBO
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, model->elementArraySize, model->elements, GL_STATIC_DRAW);
    }
    
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

- (GLuint)buildFrameBuffer {
    GLuint fb = 0;
    glGenFramebuffers(1, &fb);
    glBindFramebuffer(GL_FRAMEBUFFER, fb);
    
    glGenTextures(1, &m_colorTexture);
    glBindTexture(GL_TEXTURE_2D, m_colorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_viewWidth, m_viewHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTexture, 0);
    
    glGenTextures(1, &m_velocityTexture);
    glBindTexture(GL_TEXTURE_2D, m_velocityTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, m_viewWidth, m_viewHeight, 0, GL_RG, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_velocityTexture, 0);
    
    glGenTextures(1, &m_depthTexture);
    glBindTexture(GL_TEXTURE_2D, m_depthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, m_viewWidth, m_viewHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);
    
    GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
    glDrawBuffers(2, DrawBuffers);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        fb = 0;
    }
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    
    return fb;
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
		m_viewWidth = 1280;
		m_viewHeight = 720;
        
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
													 withNormal:YES
												   withTexcoord:NO
                                                   withColor:NO];
		
		srcDestroySource(vtxSource);
		srcDestroySource(frgSource);
		
        m_squareViewUniformIdx = glGetUniformLocation(m_squarePrgName, "viewMatrix");
		m_squareModelUniformIdx = glGetUniformLocation(m_squarePrgName, "modelMatrix");
        m_squareProjectionUniformIdx = glGetUniformLocation(m_squarePrgName, "projectionMatrix");
        m_previousMVPMatrixFirstPass = glGetUniformLocation(m_squarePrgName, "previousModelViewProjectionMatrix");
		
		if(m_squareModelUniformIdx < 0)
		{
			NSLog(@"No modelViewProjectionMatrix in square shader");
		}
        
        m_frameBuffer = [self buildFrameBuffer];
        if (m_frameBuffer == 0){
            NSLog(@"Failed to create the frame buffer");
        }
        
        filePathName = [[NSBundle mainBundle] pathForResource:@"blur" ofType:@"vsh"];
		vtxSource = srcLoadSource([filePathName cStringUsingEncoding:NSASCIIStringEncoding]);
		
		filePathName = [[NSBundle mainBundle] pathForResource:@"blur" ofType:@"fsh"];
		frgSource = srcLoadSource([filePathName cStringUsingEncoding:NSASCIIStringEncoding]);

        m_blurProgram = [self buildProgramWithVertexSource:vtxSource withFragmentSource:frgSource withNormal:NO withTexcoord:YES withColor:NO];
        m_colorTextureUniformIdx = glGetUniformLocation(m_blurProgram, "tex");
        m_depthTextureUniformIdx = glGetUniformLocation(m_blurProgram, "depthTex");
        m_velocityTextureUniformIdx = glGetUniformLocation(m_blurProgram, "velocityTex");
        m_previousModelViewProjectionUniformIdx = glGetUniformLocation(m_blurProgram, "previousModelViewProjectionMatrix");
        m_inverseModelViewMatrixIdx = glGetUniformLocation(m_blurProgram, "inverseModelViewMatrix");
        m_inverseProjectionMatrix = glGetUniformLocation(m_blurProgram, "inverseProjectionMatrix");
        
        m_screenQuad = loadScreenQuad();
        m_screenQuadVAO = [self buildVAO:m_screenQuad];
        m_screenQuadNumElements = m_screenQuad->numVertcies;
        m_screenQuadPrimType = m_screenQuad->primType;
        
        destroySquareModel(m_screenQuad);
        m_screenQuad = NULL;
		
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
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		
		// Draw our scene once without presenting the rendered image.
		//   This is done in order to pre-warm OpenGL
		// We don't need to present the buffer since we don't actually want the
		//   user to see this, we're only drawing as a pre-warm stage
		[self render];
		
		// Check for errors to make sure all of our setup went ok
		GetGLError();
	}
	
	return self;
}

-(GLuint) buildProgramWithVertexSource:(demoSource*)vertexSource
					withFragmentSource:(demoSource*)fragmentSource
							withNormal:(BOOL)hasNormal
						  withTexcoord:(BOOL)hasTexcoord
                             withColor:(BOOL)hasColor
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
    
    GetGLError();
	
	// Create a program object
	prgName = glCreateProgram();
    
    GetGLError();
	
	// Indicate the attribute indicies on which vertex arrays will be
	//  set with glVertexAttribPointer
	//  See buildVAO to see where vertex arrays are actually set
	glBindAttribLocation(prgName, POS_ATTRIB_IDX, "inPosition");

    if (hasNormal){
        glBindAttribLocation(prgName, NORMAL_ATTRIB_IDX, "inNormal");
    }
    
    if (hasTexcoord){
        glBindAttribLocation(prgName, TEXTURE_ATTRIB_IDX, "inTexture");
    }
    
    if (hasColor){
        glBindAttribLocation(prgName, COLOR_ATTRIB_IDX, "inColor");
    }
    
    GetGLError();
	
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
