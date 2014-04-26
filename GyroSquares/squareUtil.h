//
//  squareUtil.h
//  GyroSquares
//
//  Created by James Milton on 25/04/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#ifndef GyroSquares_squareUtil_h
#define GyroSquares_squareUtil_h

#include "glUtil.h"

typedef struct SquareModelStruct {
    GLuint numVertcies;
	
	GLubyte *positions;
	GLenum positionType;
	GLuint positionSize;
	GLsizei positionArraySize;
	
	GLubyte *colors;
	GLenum colorType;
	GLuint colorSize;
	GLsizei colorArraySize;
    
	GLubyte *elements;
	GLenum elementType;
	GLuint numElements;
	GLsizei elementArraySize;
    
	GLenum primType;
} SquareModel;

SquareModel *loadSquare();
SquareModel *loadBasicSquare();
void destroySquareModel(SquareModel *model);

#endif
