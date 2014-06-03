//
//  GLRenderer.cpp
//  GyroSquares
//
//  Created by James Milton on 28/05/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "GLRenderer.h"
#include "SceneColorShader.h"
#include "SceneVelocityShader.h"
#include "BlurGatherShader.h"
#include "BlurTileMaxShader.h"
#include "BlurNeighbourMaxShader.h"
#include "CubeModel.h"
#include "HollowCubeModel.h"
#include "ScreenQuadModel.h"

void GLRenderer::initOpenGL() {
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    mViewWidth = 1200;
    mViewHeight = 800;
    reshape(1200, 800);

    mCubeModel = new CubeModel;
    mCubeModel->buildVAO();
    mHollowCubeModel = new HollowCubeModel;
    mHollowCubeModel->buildVAO();
    mScreenQuadModel = new ScreenQuadModel;
    mScreenQuadModel->buildVAO();
    mBlurGatherShader = new BlurGatherShader;
    mSceneColorShader = new SceneColorShader;
    mSceneVelocityShader = new SceneVelocityShader;
    mBlurNeighbourMaxShader = new BlurNeighbourMaxShader;
    mBlurTileMaxShader = new BlurTileMaxShader;
    
    mSceneColorShader->enable();
    
    glUniformMatrix4fv(mSceneColorShader->uProjectionMatrixHandle, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
    glUniformMatrix4fv(mSceneColorShader->uViewMatrixHandle, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
    
    mSceneColorShader->disable();
    
    mCubeModel->scaleModelByVector3(1, 1, 1);
    mCubeModel->translateModelByVector3(0,0,0);
    mHollowCubeModel->scaleModelByVector3(3, 3, 0.25);
    mHollowCubeModel->translateModelByVector3(0,0,0);
    
    mExposure = 1.0f;
    mLastDt = 0.0f;
    mK = 4U;
    mHeightDividedByK = mViewHeight / mK;
    mWidthDividedByK = mViewWidth / mK;
    
    createFrameBuffers();
    render(0.0);
}

void GLRenderer::render(float dt) {
    const float FPS_CLAMP = 1.0f / 15.0f;
    if (dt > FPS_CLAMP)
        dt = FPS_CLAMP;

    drawSceneColor();
    drawSceneVelocity();
    drawBlurTileMax();
    drawBlurNeighbourMax();
    drawBlurGather();
    
    mPreviousViewMatrix = mViewMatrix;
    mLastDt = dt;
}

void GLRenderer::drawSceneColor() {
    glBindFramebuffer(GL_FRAMEBUFFER, mColorFramebuffer);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    mSceneColorShader->enable();
    mCubeModel->update(mSceneColorShader->uModelMatrixHandle);
    mCubeModel->drawElements();
    mHollowCubeModel->update(mSceneColorShader->uModelMatrixHandle);
    mHollowCubeModel->drawElements();
    mSceneColorShader->disable();
}

void GLRenderer::drawSceneVelocity() {
    GLfloat expFPS = 0.5f * mExposure / mLastDt;
    glBindFramebuffer(GL_FRAMEBUFFER, mVelocityFramebuffer);
    glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    mSceneVelocityShader->enable();
    glUniform1f(mSceneVelocityShader->ufHalfExposureXFramerateHandle, expFPS);
    glUniform1f(mSceneVelocityShader->ufKHandle, mK);
    glUniformMatrix4fv(mSceneVelocityShader->uViewMatrixHandle, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
    glUniformMatrix4fv(mSceneVelocityShader->uPreviousViewMatrixHandle, 1, GL_FALSE, glm::value_ptr(mPreviousViewMatrix));
    glUniformMatrix4fv(mSceneVelocityShader->uProjectionMatrixHandle, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
    glUniformMatrix4fv(mSceneVelocityShader->uPreviousModelMatrixHandle, 1, GL_FALSE, glm::value_ptr(mCubeModel->getPreviousModelMatrix()));
    mCubeModel->update(mSceneVelocityShader->uModelMatrixHandle);
    mCubeModel->drawElements();
    glUniformMatrix4fv(mSceneVelocityShader->uPreviousModelMatrixHandle, 1, GL_FALSE, glm::value_ptr(mHollowCubeModel->getPreviousModelMatrix()));
    mHollowCubeModel->update(mSceneVelocityShader->uModelMatrixHandle);
    mHollowCubeModel->drawElements();
    mSceneVelocityShader->disable();
}

void GLRenderer::drawBlurTileMax() {
    
}

void GLRenderer::drawBlurNeighbourMax() {
    
}

void GLRenderer::drawBlurGather() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mColorTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mVelocityTexture);
    mBlurGatherShader->enable();
    glUniform1i(mBlurGatherShader->mColorTextureHandle, 0);
    glUniform1i(mBlurGatherShader->mVelocityTextureHandle, 1);
    mScreenQuadModel->drawArrays();
    mBlurGatherShader->disable();
}

void GLRenderer::reshape(int width, int height) {
    mViewHeight = width;
    mViewHeight = height;
    mHeightDividedByK = height / mK;
    mWidthDividedByK = width / mK;
    mProjectionMatrix = glm::perspective(45.0f, (float)width/(float)height, 0.1f, 100.0f);
    mViewMatrix = glm::lookAt(glm::vec3(0,0,20), glm::vec3(0,0,0), glm::vec3(0,1,0));
}

int velMod = 1500;
void GLRenderer::leap_rightHandVelocity(float x, float y, float z) {
    mCubeModel->velocityVector.x += x/velMod;
    mCubeModel->velocityVector.y += y/velMod;
    mCubeModel->velocityVector.z += z/velMod;
}

void GLRenderer::leap_leftHandVelocity(float x, float y, float z) {
    mHollowCubeModel->velocityVector.x += x/velMod;
    mHollowCubeModel->velocityVector.y += y/velMod;
    mHollowCubeModel->velocityVector.z += z/velMod;
}

void GLRenderer::resetFramebuffers() {
    if (mColorFramebuffer){
        glDeleteFramebuffers(1, &mColorFramebuffer);
        mColorFramebuffer = -1;
    }
    
    if (mVelocityFramebuffer){
        glDeleteBuffers(1, &mVelocityFramebuffer);
        mVelocityFramebuffer = -1;
    }
    
    if (mTileMaxFramebuffer){
        glDeleteBuffers(1, &mTileMaxFramebuffer);
        mTileMaxFramebuffer = -1;
    }
    
    if (mNeighbourMaxFramebuffer){
        glDeleteBuffers(1, &mNeighbourMaxFramebuffer);
        mNeighbourMaxFramebuffer = -1;
    }
    
    if (mColorTexture){
        glDeleteTextures(1, &mColorTexture);
        mColorTexture = -1;
    }
    
    if (mVelocityTexture){
        glDeleteTextures(1, &mVelocityTexture);
        mVelocityTexture = -1;
    }
    
    if (mTileMaxTexture){
        glDeleteTextures(1, &mTileMaxTexture);
        mTileMaxTexture = -1;
    }
    
    if (mNeighbourMaxTexture){
        glDeleteTextures(1, &mNeighbourMaxTexture);
        mNeighbourMaxTexture = -1;
    }
    
    if (mDepthTexture){
        glDeleteTextures(1, &mDepthTexture);
        mDepthTexture = -1;
    }
}

void GLRenderer::createFrameBuffers() {
    resetFramebuffers();
    
    glGenFramebuffers(1, &mColorFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mColorFramebuffer);
    
    glGenTextures(1, &mColorTexture);
    glBindTexture(GL_TEXTURE_2D, mColorTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mViewWidth, mViewHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorTexture, 0);
    
    glGenTextures(1, &mDepthTexture);
    glBindTexture(GL_TEXTURE_2D, mDepthTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, mViewWidth, mViewHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthTexture, 0);
    
    GLenum DrawBuffers[] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        fprintf (stderr, "ERROR: incomplete color framebuffer\n");
    } else {
        printf("color framebuffer = %i\n", mColorFramebuffer);
    }
    
    glGenFramebuffers(1, &mVelocityFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mVelocityFramebuffer);
    
    glGenTextures(1, &mVelocityTexture);
    glBindTexture(GL_TEXTURE_2D, mVelocityTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, mViewWidth, mViewHeight, 0, GL_RG, GL_UNSIGNED_BYTE, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mVelocityTexture, 0);
    
    glDrawBuffers(1, DrawBuffers);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        fprintf (stderr, "ERROR: incomplete velocity framebuffer\n");
    } else {
        printf("velocity framebuffer = %i\n", mVelocityFramebuffer);
    }
    
    glGenFramebuffers(1, &mTileMaxFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mTileMaxFramebuffer);
    
    glGenTextures(1, &mTileMaxTexture);
    glBindTexture(GL_TEXTURE_2D, mTileMaxTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, mViewWidth, mViewHeight, 0, GL_RG, GL_UNSIGNED_BYTE, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTileMaxTexture, 0);
    
    glDrawBuffers(1, DrawBuffers);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        fprintf (stderr, "ERROR: incomplete TileMax framebuffer\n");
    } else {
        printf("TileMax framebuffer = %i\n", mTileMaxFramebuffer);
    }
    
    glGenFramebuffers(1, &mNeighbourMaxFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, mNeighbourMaxFramebuffer);
    
    glGenTextures(1, &mNeighbourMaxTexture);
    glBindTexture(GL_TEXTURE_2D, mNeighbourMaxTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, mViewWidth, mViewHeight, 0, GL_RG, GL_UNSIGNED_BYTE, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mNeighbourMaxTexture, 0);
    
    glDrawBuffers(1, DrawBuffers);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        fprintf (stderr, "ERROR: incomplete NeighbourMax framebuffer\n");
    } else {
        printf("NeighbourMax framebuffer = %i\n", mNeighbourMaxFramebuffer);
    }

    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}