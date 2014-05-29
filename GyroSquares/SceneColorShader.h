//
//  SceneColorShader.h
//  GyroSquares
//
//  Created by James Milton on 29/05/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "BaseShader.h"

class SceneColorShader : public BaseShader {
public:
    SceneColorShader();
    
    GLint uModelMatrixHandle;
    GLint uViewMatrixHandle;
    GLint uProjectionMatrixHandle;
};
