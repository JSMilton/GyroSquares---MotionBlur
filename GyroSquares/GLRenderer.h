//
//  GLRenderer.h
//  GyroSquares
//
//  Created by James Milton on 28/05/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glUtil.h"

class SceneColorShader;
class SceneVelocityShader;
class BlurGatherShader;
class BlurTileMaxShader;
class BlurNeighbourMaxShader;
class CubeModel;
class HollowCubeModel;
class ScreenQuadModel;

class GLRenderer {
public:
    void initOpenGL();
    void render(float dt);
    void reshape(int width, int height, bool isLive);
    void destroy();
    void leap_rightHandVelocity(float x, float y, float z);
    void leap_leftHandVelocity(float x, float y, float z);
    
private:
    void createFrameBuffers();
    void resetFramebuffers();
    void drawSceneColor();
    void drawSceneVelocity();
    void drawBlurGather();
    void drawBlurTileMax();
    void drawBlurNeighbourMax();
    void computeMaxSampleTapDistance();
    void freeGLBindings() const;
    
    SceneColorShader *mSceneColorShader;
    SceneVelocityShader *mSceneVelocityShader;
    BlurGatherShader *mBlurGatherShader;
    BlurTileMaxShader *mBlurTileMaxShader;
    BlurNeighbourMaxShader *mBlurNeighbourMaxShader;
    CubeModel *mCubeModel;
    HollowCubeModel *mHollowCubeModel;
    ScreenQuadModel *mScreenQuadModel;
    
    glm::mat4 mProjectionMatrix;
    glm::mat4 mViewMatrix;
    glm::mat4 mPreviousViewMatrix;
    
    GLuint mColorTexture;
    GLuint mVelocityTexture;
    GLuint mDepthTexture;
    GLuint mTileMaxTexture;
    GLuint mNeighbourMaxTexture;
    GLuint mRandomTexture;
    
    GLuint mColorFramebuffer;
    GLuint mVelocityFramebuffer;
    GLuint mTileMaxFramebuffer;
    GLuint mNeighbourMaxFramebuffer;
    GLuint mSmallRenderbuffer;
    GLuint mNormalRenderbuffer;
    
    int mViewWidth;
    int mViewHeight;
    int mHeightDividedByK;
    int mWidthDividedByK;
    
    float mExposure;
    float mLastDt;
    
    uint32_t mK;
    uint32_t mLastK;
    
    int32_t mMaxSampleTapDistance;
    
    bool mPaused;
};
