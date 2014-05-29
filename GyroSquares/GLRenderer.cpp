//
//  GLRenderer.cpp
//  GyroSquares
//
//  Created by James Milton on 28/05/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "GLRenderer.h"
#include "SceneColorShader.h"
#include "CubeModel.h"

void GLRenderer::initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    
    reshape(1200, 800);

    mCubeModel = new CubeModel;
    mSceneColorShader = new SceneColorShader;
    
    glUniformMatrix4fv(mSceneColorShader->uProjectionMatrixHandle, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
    glUniformMatrix4fv(mSceneColorShader->uViewMatrixHandle, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
}

void GLRenderer::render() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    mCubeModel->translateModelByVector3(glm::vec3(0,0,-2));
    glUniformMatrix4fv(mSceneColorShader->uModelMatrixHandle, 1, GL_FALSE, glm::value_ptr(mCubeModel->getModelMatrix()));
    mCubeModel->drawElements();
}

void GLRenderer::reshape(int width, int height) {
    mViewHeight = width;
    mViewHeight = height;
    mProjectionMatrix = glm::perspective(45.0f, (float)width/height, 0.1f, 100.0f);
    mViewMatrix = glm::lookAt(glm::vec3(0,0,1), glm::vec3(0,0,0), glm::vec3(0,1,0));
}