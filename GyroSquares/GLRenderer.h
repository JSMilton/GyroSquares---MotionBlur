//
//  GLRenderer.h
//  GyroSquares
//
//  Created by James Milton on 28/05/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "glm.hpp"
class SceneColorShader;
class CubeModel;

class GLRenderer {
public:
    GLRenderer();
    
    void initGL();
    void render();
    void reshape(int width, int height);
    void destroy();
    void moveInnerCube(glm::vec3 vector);
    void moveOuterFrame(glm::vec3 vector);
    
private:
    SceneColorShader *myShader;
    CubeModel *myCube;
    
    glm::mat4 mProjectionMatrix;
    glm::mat4 mViewMatrix;
};
