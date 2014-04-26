//
//  OpenGLRenderer.m
//  GyroSquares
//
//  Created by James Milton on 25/04/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#import "OpenGLRenderer.h"

#import "matrixUtil.h"
#import "sourceUtil.h"


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
	COLOR_ATTRIB_IDX,
};

@implementation OpenGLRenderer
{
    GLuint m_characterPrgName;
    GLint m_characterMvpUniformIdx;
    GLuint m_characterVAOName;
    GLuint m_characterTexName;
    GLenum m_characterPrimType;
    GLenum m_characterElementType;
    GLuint m_characterNumElements;
    GLfloat m_characterAngle;
    
    
    GLuint m_viewWidth;
    GLuint m_viewHeight;
    
    GLboolean m_useVBOs;
}

- (GLuint)buildVAO {
    return 0;
}

- (id) initWithDefaultFBO: (GLuint) defaultFBOName
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
		m_characterAngle = 0;
        
		//////////////////////////////
		// Create Vertices          //
		//////////////////////////////
		
		
		// Build Vertex Buffer Objects (VBOs) and Vertex Array Object (VAOs) with our model data: Mine will be a cube, not an awesome monster :(
		m_characterVAOName = [self buildVAO];
		
		// Cache the number of element and primType to use later in our glDrawElements calls
		m_characterNumElements = m_characterModel->numElements;
		m_characterPrimType = m_characterModel->primType;
		m_characterElementType = m_characterModel->elementType;
        
		if(m_useVBOs)
		{
			//If we're using VBOs we can destroy all this memory since buffers are
			// loaded into GL and we've saved anything else we need
			mdlDestroyModel(m_characterModel);
			m_characterModel = NULL;
		}
        
		
		////////////////////////////////////
		// Load texture for our character //
		////////////////////////////////////
		
		filePathName = [[NSBundle mainBundle] pathForResource:@"demon" ofType:@"png"];
		demoImage *image = imgLoadImage([filePathName cStringUsingEncoding:NSASCIIStringEncoding], false);
		
		// Build a texture object with our image data
		m_characterTexName = [self buildTexture:image];
		
		// We can destroy the image once it's loaded into GL
		imgDestroyImage(image);
        
		
		////////////////////////////////////////////////////
		// Load and Setup shaders for character rendering //
		////////////////////////////////////////////////////
		
		demoSource *vtxSource = NULL;
		demoSource *frgSource = NULL;
		
		filePathName = [[NSBundle mainBundle] pathForResource:@"character" ofType:@"vsh"];
		vtxSource = srcLoadSource([filePathName cStringUsingEncoding:NSASCIIStringEncoding]);
		
		filePathName = [[NSBundle mainBundle] pathForResource:@"character" ofType:@"fsh"];
		frgSource = srcLoadSource([filePathName cStringUsingEncoding:NSASCIIStringEncoding]);
		
		// Build Program
		m_characterPrgName = [self buildProgramWithVertexSource:vtxSource
											 withFragmentSource:frgSource
													 withNormal:NO
												   withTexcoord:YES];
		
		srcDestroySource(vtxSource);
		srcDestroySource(frgSource);
		
		m_characterMvpUniformIdx = glGetUniformLocation(m_characterPrgName, "modelViewProjectionMatrix");
		
		if(m_characterMvpUniformIdx < 0)
		{
			NSLog(@"No modelViewProjectionMatrix in character shader");
		}
		
		
#if RENDER_REFLECTION
		
		m_reflectWidth = 512;
		m_reflectHeight = 512;
		
		////////////////////////////////////////////////
		// Load a model for a quad for the reflection //
		////////////////////////////////////////////////
		
		m_quadModel = mdlLoadQuadModel();
		// Build Vertex Buffer Objects (VBOs) and Vertex Array Object (VAOs) with our model data
		m_reflectVAOName = [self buildVAO:m_quadModel];
		
		// Cache the number of element and primType to use later in our glDrawElements calls
		m_quadNumElements = m_quadModel->numElements;
		m_quadPrimType    = m_quadModel->primType;
		m_quadElementType = m_quadModel->elementType;
		
		if(m_useVBOs)
		{
			//If we're using VBOs we can destroy all this memory since buffers are
			// loaded into GL and we've saved anything else we need
			mdlDestroyModel(m_quadModel);
			m_quadModel = NULL;
		}
		
		/////////////////////////////////////////////////////
		// Create texture and FBO for reflection rendering //
		/////////////////////////////////////////////////////
		
		m_reflectFBOName = [self buildFBOWithWidth:m_reflectWidth andHeight:m_reflectHeight];
		
		// Get the texture we created in buildReflectFBO by binding the
		// reflection FBO and getting the buffer attached to color 0
		glBindFramebuffer(GL_FRAMEBUFFER, m_reflectFBOName);
		
		GLint iReflectTexName;
		
		glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                                              GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME,
                                              &iReflectTexName);
		
		m_reflectTexName = ((GLuint*)(&iReflectTexName))[0];
		
		/////////////////////////////////////////////////////
		// Load and setup shaders for reflection rendering //
		/////////////////////////////////////////////////////
		
		filePathName = [[NSBundle mainBundle] pathForResource:@"reflect" ofType:@"vsh"];
		vtxSource = srcLoadSource([filePathName cStringUsingEncoding:NSASCIIStringEncoding]);
		
		filePathName = [[NSBundle mainBundle] pathForResource:@"reflect" ofType:@"fsh"];
		frgSource = srcLoadSource([filePathName cStringUsingEncoding:NSASCIIStringEncoding]);
		
		// Build Program
		m_reflectPrgName = [self buildProgramWithVertexSource:vtxSource
										   withFragmentSource:frgSource
												   withNormal:YES
												 withTexcoord:NO];
		
		srcDestroySource(vtxSource);
		srcDestroySource(frgSource);
		
		m_reflectModelViewUniformIdx = glGetUniformLocation(m_reflectPrgName, "modelViewMatrix");
		
		if(m_reflectModelViewUniformIdx < 0)
		{
			NSLog(@"No modelViewMatrix in reflection shader");
		}
		
		m_reflectProjectionUniformIdx = glGetUniformLocation(m_reflectPrgName, "modelViewProjectionMatrix");
		
		if(m_reflectProjectionUniformIdx < 0)
		{
			NSLog(@"No modelViewProjectionMatrix in reflection shader");
		}
		
		m_reflectNormalMatrixUniformIdx = glGetUniformLocation(m_reflectPrgName, "normalMatrix");
		
		if(m_reflectNormalMatrixUniformIdx < 0)
		{
			NSLog(@"No normalMatrix in reflection shader");
		}
#endif // RENDER_REFLECTION
		
		////////////////////////////////////////////////
		// Set up OpenGL state that will never change //
		////////////////////////////////////////////////
		
		// Depth test will always be enabled
		glEnable(GL_DEPTH_TEST);
        
		// We will always cull back faces for better performance
		glEnable(GL_CULL_FACE);
		
		// Always use this clear color
		glClearColor(0.5f, 0.4f, 0.5f, 1.0f);
		
		// Draw our scene once without presenting the rendered image.
		//   This is done in order to pre-warm OpenGL
		// We don't need to present the buffer since we don't actually want the
		//   user to see this, we're only drawing as a pre-warm stage
		[self render];
		
		// Reset the m_characterAngle which is incremented in render
		m_characterAngle = 0;
		
		// Check for errors to make sure all of our setup went ok
		GetGLError();
	}
	
	return self;
}

@end
