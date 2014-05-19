//
//  OpenGLRenderer.h
//  GyroSquares
//
//  Created by James Milton on 25/04/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "glUtil.h"
#import <GLKit/GLKit.h>
#import <Foundation/Foundation.h>

@interface OpenGLRenderer : NSObject
{
	GLuint m_defaultFBOName;
}

- (id)initWithDefaultFBO: (GLuint) defaultFBOName;
- (void)resizeWithWidth:(GLuint)width AndHeight:(GLuint)height;
- (void)render;
- (void)dealloc;
- (void)moveCamera:(GLKVector3)vector;
- (void)moveInnerCube:(GLKVector3)vector;
- (void)moveOuterFrame:(GLKVector3)vector;

@end
