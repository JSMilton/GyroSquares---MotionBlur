//
//  SceneVelocityShader.cpp
//  GyroSquares
//
//  Created by James Milton on 02/06/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "SceneVelocityShader.h"

SceneVelocityShader::SceneVelocityShader() : BaseShader("sceneVelocity", "sceneVelocity") {
    uModelMatrixHandle = getUniformLocation("uModelMatrix");
    uViewMatrixHandle = getUniformLocation("uViewMatrix");
    uPreviousModelMatrixHandle = getUniformLocation("uPreviousModelMatrix");
    uPreviousViewMatrixHandle = getUniformLocation("uPreviousViewMatrix");
    uProjectionMatrixHandle = getUniformLocation("uProjectionMatrix");
    ufHalfExposureXFramerateHandle = getUniformLocation("ufHalfExposureXFramerate");
    ufKHandle = getUniformLocation("ufK");
}