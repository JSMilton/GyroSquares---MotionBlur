//
//  SceneVelocityShader.h
//  GyroSquares
//
//  Created by James Milton on 02/06/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "BaseShader.h"

class SceneVelocityShader : public BaseShader {
public:
    SceneVelocityShader();
    
    GLint uModelMatrixHandle;
    GLint uViewMatrixHandle;
    GLint uPreviousModelMatrixHandle;
    GLint uPreviousViewMatrixHandle;
    GLint uProjectionMatrixHandle;
    GLint ufHalfExposureXFramerateHandle;
    GLint ufKHandle;
};
