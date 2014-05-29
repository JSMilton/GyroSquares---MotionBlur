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

class SceneColorShader;
class CubeModel;

class GLRenderer {
public:
    void initOpenGL();
    void render();
    void reshape(int width, int height);
    void destroy();
    void leap_rightHandVelocity(float x, float y, float z);
    void leap_leftHandVelocity(float x, float y, float z);
    
private:
    SceneColorShader *mSceneColorShader;
    CubeModel *mCubeModel;
    
    glm::mat4 mProjectionMatrix;
    glm::mat4 mViewMatrix;
    
    int mViewWidth;
    int mViewHeight;
};
