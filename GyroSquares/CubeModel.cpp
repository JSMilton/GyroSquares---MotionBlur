//
//  CubeModel.cpp
//  GyroSquares
//
//  Created by James Milton on 29/05/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "CubeModel.h"

CubeModel::CubeModel() : BaseModel() {
    const int vertexCount = 24;
    
    GLfloat positionArray[] = {
        // Front
        1.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,
        1.0, -1.0,  1.0,
        -1.0, -1.0,  1.0,
        
        // Back
        1.0,  1.0, -1.0,
        -1.0,  1.0, -1.0,
        1.0, -1.0, -1.0,
        -1.0, -1.0, -1.0,
        
        // Right
        1.0,  1.0,  1.0,
        1.0,  1.0, -1.0,
        1.0, -1.0,  1.0,
        1.0, -1.0, -1.0,
        
        // Left
        -1.0,  1.0,  1.0,
        -1.0,  1.0, -1.0,
        -1.0, -1.0,  1.0,
        -1.0, -1.0, -1.0,
        
        // Top
        1.0,  1.0,  1.0,
        1.0,  1.0, -1.0,
        -1.0,  1.0,  1.0,
        -1.0,  1.0, -1.0,
        
        // Bottom
        1.0, -1.0,  1.0,
        1.0, -1.0, -1.0,
        -1.0, -1.0,  1.0,
        -1.0, -1.0, -1.0,
    };
    
    GLfloat normalArray[] = {
        // Front
        0.0,   0.0,   1.0,
        0.0,   0.0,   1.0,
        0.0,   0.0,   1.0,
        0.0,   0.0,   1.0,
        
        // Back
        0.0,   0.0,   -1.0,
        0.0,   0.0,   -1.0,
        0.0,   0.0,   -1.0,
        0.0,   0.0,   -1.0,
        
        // Right
        1.0,   0.0,   0.0,
        1.0,   0.0,   0.0,
        1.0,   0.0,   0.0,
        1.0,   0.0,   0.0,
        
        // Left
        -1.0,   0.0,   0.0,
        -1.0,   0.0,   0.0,
        -1.0,   0.0,   0.0,
        -1.0,   0.0,   0.0,
        
        // Top
        0.0,   1.0,   0.0,
        0.0,   1.0,   0.0,
        0.0,   1.0,   0.0,
        0.0,   1.0,   0.0,
        
        // Bottom
        0.0,   -1.0,   0.0,
        0.0,   -1.0,   0.0,
        0.0,   -1.0,   0.0,
        0.0,   -1.0,   0.0,
    };
    
    GLfloat colorArray[] = {
        // Front
        1.0,   1.0,   1.0, 1.0,
        1.0,   1.0,   1.0, 1.0,
        1.0,   1.0,   1.0, 1.0,
        1.0,   1.0,   1.0, 1.0,
        
        // Back
        0.0,   0.0,   0.0, 1.0,
        0.0,   0.0,   0.0, 1.0,
        0.0,   0.0,   0.0, 1.0,
        0.0,   0.0,   0.0, 1.0,
        
        // Right
        1.0,   0.0,   0.0, 1.0,
        1.0,   0.0,   0.0, 1.0,
        1.0,   0.0,   0.0, 1.0,
        1.0,   0.0,   0.0, 1.0,
        
        // Left
        0.0,   1.0,   0.0, 1.0,
        0.0,   1.0,   0.0, 1.0,
        0.0,   1.0,   0.0, 1.0,
        0.0,   1.0,   0.0, 1.0,
        
        // Top
        0.0,   0.0,   1.0, 1.0,
        0.0,   0.0,   1.0, 1.0,
        0.0,   0.0,   1.0, 1.0,
        0.0,   0.0,   1.0, 1.0,
        
        // Bottom
        1.0,   1.0,   0.0, 1.0,
        1.0,   1.0,   0.0, 1.0,
        1.0,   1.0,   0.0, 1.0,
        1.0,   1.0,   0.0, 1.0
    };
    
    GLushort elementArray[] = {
        // Front
        0, 1, 2,
        1, 2, 3,
        
        // Back
        4, 5, 6,
        5, 6, 7,
        
        // Top
        8, 9, 10,
        9, 10, 11,
        
        // Bottom
        12, 13, 14,
        13, 14, 15,
        
        // Left
        16, 17, 18,
        17, 18, 19,
        
        // Right
        20, 21, 22,
        21, 22, 23
    };
    
    mPositionSize = 3;
    mPositionArraySize = sizeof(positionArray);
    mPositionType = GL_FLOAT;
    mPositions = (GLubyte *)malloc(mPositionArraySize);
    memcpy(mPositions, positionArray, mPositionArraySize);
    
    mNormalSize = 3;
    mNormalArraySize = sizeof(normalArray);
    mNormalType = GL_FLOAT;
    mNormals = (GLubyte *)malloc(mNormalArraySize);
    memcpy(mNormals, normalArray, mNormalArraySize);
    
    mColorSize = 4;
    mColorArraySize = sizeof(colorArray);
    mColorType = GL_FLOAT;
    mColors = (GLubyte *)malloc(mColorArraySize);
    memcpy(mColors, colorArray, mColorArraySize);
    
    mPrimType = GL_TRIANGLES;
    
    mNumElements = sizeof(elementArray) / sizeof(GLushort);
    mElementType = GL_UNSIGNED_SHORT;
    mElementArraySize = sizeof(elementArray);
    mElements = (GLubyte *)malloc(mElementArraySize);
    memcpy(mElements, elementArray, mElementArraySize);
    
    mNumVertcies = vertexCount;
}

void CubeModel::update(GLint modelMatrixHandle) {
    velocityVector.x *= 0.99;
    velocityVector.y *= 0.99;
    velocityVector.z *= 0.99;
    
    rotateModelByVector3AndAngle(1, 0, 0, velocityVector.x);
    rotateModelByVector3AndAngle(0, 1, 0, velocityVector.y);
    
    glm::mat4 modelMatrix = BaseModel::createModelMatrix();
    
    glUniformMatrix4fv(modelMatrixHandle, 1, GL_FALSE, glm::value_ptr(BaseModel::createModelMatrix()));
    
    mPreviousModelMatrix = modelMatrix;
}
