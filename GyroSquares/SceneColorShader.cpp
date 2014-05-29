//
//  SceneColorShader.cpp
//  GyroSquares
//
//  Created by James Milton on 29/05/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "SceneColorShader.h"

SceneColorShader::SceneColorShader() : BaseShader("sceneColor", "sceneColor") {
    uModelMatrixHandle = getUniformLocation("uModelMatrix");
    uViewMatrixHandle = getUniformLocation("uViewMatrix");
    uProjectionMatrixHandle = getUniformLocation("uProjectionMatrix");
}