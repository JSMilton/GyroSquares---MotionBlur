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

#define RANDOM_TEXTURE_SIZE (256U)

void GLRenderer::initOpenGL() {
    mExposure = 1.5f;
    mLastDt = 0.0f;
    mK = 4U;
    mMaxSampleTapDistance = 6;
    mHeightDividedByK = mViewHeight / mK;
    mWidthDividedByK = mViewWidth / mK;
    
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.f, 0.f, 0.f, 1.0f);
    mViewWidth = 1200;
    mViewHeight = 800;
    reshape(1200, 800, false);

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
    
    mCubeModel->scaleModelByVector3(1, 1, 1);
    mCubeModel->translateModelByVector3(0,0,0);
    mHollowCubeModel->scaleModelByVector3(3, 3, 0.25);
    mHollowCubeModel->translateModelByVector3(0,0,0);
    
    render(0.0);
}

void GLRenderer::render(float dt) {
    const float FPS_CLAMP = 1.0f / 15.0f;
    if (dt > FPS_CLAMP)
        dt = FPS_CLAMP;
    
    freeGLBindings();
    
    if (!mPaused){
        drawSceneColor();
        drawSceneVelocity();
        drawBlurTileMax();
        drawBlurNeighbourMax();
        drawBlurGather();
        freeGLBindings();
    }
    
    mPreviousViewMatrix = mViewMatrix;
    mLastDt = dt;
}

void GLRenderer::drawSceneColor() {
    glBindFramebuffer(GL_FRAMEBUFFER, mColorFramebuffer);
    glViewport(0, 0, mViewWidth, mViewHeight);
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    mSceneColorShader->enable();
    glUniformMatrix4fv(mSceneColorShader->uProjectionMatrixHandle, 1, GL_FALSE, glm::value_ptr(mProjectionMatrix));
    glUniformMatrix4fv(mSceneColorShader->uViewMatrixHandle, 1, GL_FALSE, glm::value_ptr(mViewMatrix));
    mCubeModel->update(mSceneColorShader->uModelMatrixHandle);
    mCubeModel->drawElements();
    mHollowCubeModel->update(mSceneColorShader->uModelMatrixHandle);
    mHollowCubeModel->drawElements();
    mSceneColorShader->disable();
}

void GLRenderer::drawSceneVelocity() {
    GLfloat expFPS = 0.5f * mExposure / mLastDt;
    glBindFramebuffer(GL_FRAMEBUFFER, mVelocityFramebuffer);
    glViewport(0, 0, mViewWidth, mViewHeight);
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
    glBindFramebuffer(GL_FRAMEBUFFER, mTileMaxFramebuffer);
    glViewport(0, 0, mWidthDividedByK, mHeightDividedByK);
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mBlurTileMaxShader->enable();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mVelocityTexture);
    glUniform1i(mBlurTileMaxShader->mInputTextureHandle, 0);
    glUniform1i(mBlurTileMaxShader->mKHandle, mK);
    mScreenQuadModel->drawArrays();
    mBlurTileMaxShader->disable();
}

void GLRenderer::drawBlurNeighbourMax() {
    glBindFramebuffer(GL_FRAMEBUFFER, mNeighbourMaxFramebuffer);
    glViewport(0, 0, mWidthDividedByK, mHeightDividedByK);
    glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mBlurNeighbourMaxShader->enable();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mTileMaxTexture);
    glUniform1i(mBlurNeighbourMaxShader->mInputTextureHandle, 0);
    mScreenQuadModel->drawArrays();
    mBlurNeighbourMaxShader->disable();
}

void GLRenderer::computeMaxSampleTapDistance(void)
{
    mMaxSampleTapDistance = (2 * mViewHeight + 1056) / 416;
}

void GLRenderer::drawBlurGather() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, mViewWidth, mViewHeight);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mColorTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, mVelocityTexture);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, mNeighbourMaxTexture);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, mDepthTexture);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, mRandomTexture);
    mBlurGatherShader->enable();
    
    glUniform1i(mBlurGatherShader->mKHandle, mK);
    glUniform1i(mBlurGatherShader->mSHandle, 15);
    glUniform1f(mBlurGatherShader->mHalfExposureHandle,
                static_cast<GLfloat>(mExposure * 0.5f));
    glUniform1f(mBlurGatherShader->mMaxSampleTapDistanceHandle,
                static_cast<GLfloat>(mMaxSampleTapDistance));
    glUniform1i(mBlurGatherShader->mColorTextureHandle, 0);
    glUniform1i(mBlurGatherShader->mVelocityTextureHandle, 1);
    glUniform1i(mBlurGatherShader->mNeighbourMaxTextureHandle, 2);
    glUniform1i(mBlurGatherShader->mDepthTextureHandle, 3);
    glUniform1i(mBlurGatherShader->mRandomTextureHandle, 4);
    mScreenQuadModel->drawArrays();
    mBlurGatherShader->disable();
}

void GLRenderer::reshape(int width, int height, bool isLive) {
    glViewport(0, 0, width, height);
    mViewWidth = width;
    mViewHeight = height;
    mHeightDividedByK = height / mK;
    mWidthDividedByK = width / mK;
    mProjectionMatrix = glm::perspective(45.0f, (float)width/(float)height, 0.1f, 100.0f);
    mViewMatrix = glm::lookAt(glm::vec3(0,0,20), glm::vec3(0,0,0), glm::vec3(0,1,0));
    computeMaxSampleTapDistance();
    
    if (!isLive){
        createFrameBuffers();
    }
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

void GLRenderer::freeGLBindings(void) const
{
    glBindFramebuffer(GL_FRAMEBUFFER,     0);
    glBindRenderbuffer(GL_RENDERBUFFER,   0);
    glBindBuffer(GL_ARRAY_BUFFER,         0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindTexture(GL_TEXTURE_2D,          0);
    glBindTexture(GL_TEXTURE_CUBE_MAP,    0);
}

void GLRenderer::resetFramebuffers() {
    glDeleteFramebuffers(1, &mColorFramebuffer);
    glDeleteFramebuffers(1, &mVelocityFramebuffer);
    glDeleteFramebuffers(1, &mTileMaxFramebuffer);
    glDeleteFramebuffers(1, &mNeighbourMaxFramebuffer);
    
    glDeleteRenderbuffers(1, &mSmallRenderbuffer);
    glDeleteRenderbuffers(1, &mNormalRenderbuffer);
    
    glDeleteTextures(1, &mColorTexture);
    glDeleteTextures(1, &mVelocityTexture);
    glDeleteTextures(1, &mTileMaxTexture);
    glDeleteTextures(1, &mNeighbourMaxTexture);
    glDeleteTextures(1, &mDepthTexture);
}

void GLRenderer::createFrameBuffers() {
    resetFramebuffers();
    
    glGenRenderbuffers(1, &mNormalRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mNormalRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                          mViewWidth, mViewHeight);
    
    glGenRenderbuffers(1, &mSmallRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, mSmallRenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24,
                          mWidthDividedByK, mHeightDividedByK);
    
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, mViewWidth, mViewHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_INT, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
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
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, mNormalRenderbuffer);
    
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, mWidthDividedByK, mHeightDividedByK, 0, GL_RG, GL_UNSIGNED_BYTE, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mTileMaxTexture, 0);
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, mSmallRenderbuffer);
    
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
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RG, mWidthDividedByK, mHeightDividedByK, 0, GL_RG, GL_UNSIGNED_BYTE, NULL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mNeighbourMaxTexture, 0);
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
                              GL_RENDERBUFFER, mSmallRenderbuffer);
    
    glDrawBuffers(1, DrawBuffers);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
        fprintf (stderr, "ERROR: incomplete NeighbourMax framebuffer\n");
    } else {
        printf("NeighbourMax framebuffer = %i\n", mNeighbourMaxFramebuffer);
    }
    
    // Random texture (used in gather pass)
    srand( static_cast<unsigned int>( time(NULL) ) );
    GLubyte *r = new unsigned char[RANDOM_TEXTURE_SIZE * RANDOM_TEXTURE_SIZE];
    for(unsigned int i = 0U; i < RANDOM_TEXTURE_SIZE; i++)
    {
        for(unsigned int j = 0U; j < RANDOM_TEXTURE_SIZE; j++)
        {
            GLubyte val = static_cast<GLubyte>( rand() ) &
            static_cast<GLubyte>(0x00ff);
            r[i * RANDOM_TEXTURE_SIZE + j] = val;
        }
    }

    glGenTextures(1, &mRandomTexture);
    glBindTexture(GL_TEXTURE_2D, mRandomTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
                 RANDOM_TEXTURE_SIZE, RANDOM_TEXTURE_SIZE, 0, GL_RED,
                 GL_UNSIGNED_BYTE, static_cast<const GLvoid*>(r));
    delete r;
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}