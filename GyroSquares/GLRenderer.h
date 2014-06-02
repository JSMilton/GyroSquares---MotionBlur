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
class CubeModel;
class HollowCubeModel;
class ScreenQuadModel;

class GLRenderer {
public:
    void initOpenGL();
    void render();
    void reshape(int width, int height);
    void destroy();
    void leap_rightHandVelocity(float x, float y, float z);
    void leap_leftHandVelocity(float x, float y, float z);
    
private:
    void createFrameBuffers();
    void resetFramebuffers();
    
    SceneColorShader *mSceneColorShader;
    SceneVelocityShader *mSceneVelocityShader;
    BlurGatherShader *mBlurGatherShader;
    CubeModel *mCubeModel;
    HollowCubeModel *mHollowCubeModel;
    ScreenQuadModel *mScreenQuadModel;
    
    glm::mat4 mProjectionMatrix;
    glm::mat4 mViewMatrix;
    glm::mat4 mPreviousProjectionMatrix;
    glm::mat4 mPreviousViewMatrix;
    
    GLuint mColorTexture;
    GLuint mVelocityTexture;
    GLuint mDepthTexture;
    
    GLuint mColorFramebuffer;
    GLuint mVelocityFramebuffer;
    
    int mViewWidth;
    int mViewHeight;
};
