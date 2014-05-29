//
//  GLRenderer.cpp
//  GyroSquares
//
//  Created by James Milton on 28/05/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "GLRenderer.h"
#include "SceneColorShader.h"

GLRenderer::GLRenderer() {
    myShader = new SceneColorShader();
}