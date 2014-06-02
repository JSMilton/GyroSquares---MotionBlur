//
//  HollowCubeModel.cpp
//  GyroSquares
//
//  Created by James Milton on 02/06/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "HollowCubeModel.h"

HollowCubeModel::HollowCubeModel() : BaseModel() {
    const int vertexCount = 56;
    
    GLfloat m1;
    m1 = 0.75;
    
    GLfloat positionArray[] = {
        
        // Front
        1.0,  1.0,  1.0,
        0.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,
        m1,   m1,   1.0,
        -m1,   m1,   1.0,
        1.0,  0.0,  1.0,
        -1.0,  0.0,  1.0,
        m1,  -m1,   1.0,
        -m1,  -m1,   1.0,
        1.0, -1.0,  1.0,
        0.0, -1.0,  1.0,
        -1.0, -1.0,  1.0,
        
        // Back
        1.0,  1.0, -1.0,
        0.0,  1.0, -1.0,
        -1.0,  1.0, -1.0,
        m1,   m1,  -1.0,
        -m1,   m1,  -1.0,
        1.0,  0.0, -1.0,
        -1.0,  0.0, -1.0,
        m1,  -m1,  -1.0,
        -m1,  -m1,  -1.0,
        1.0, -1.0, -1.0,
        0.0, -1.0, -1.0,
        -1.0, -1.0, -1.0,
        
        // Left
        -1.0, 1.0, 1.0,
        -1.0, 1.0, -1.0,
        -1.0, -1.0, 1.0,
        -1.0, -1.0, -1.0,
        
        // Right
        1.0, 1.0, 1.0,
        1.0, 1.0, -1.0,
        1.0, -1.0, 1.0,
        1.0, -1.0, -1.0,
        
        // Top
        -1.0, 1.0, 1.0,
        -1.0, 1.0, -1.0,
        1.0, 1.0, 1.0,
        1.0, 1.0, -1.0,
        
        // Bottom
        -1.0, -1.0, 1.0,
        -1.0, -1.0, -1.0,
        1.0, -1.0, 1.0,
        1.0, -1.0, -1.0,
        
        // Middle Top
        -m1, m1, 1.0,
        -m1, m1, -1.0,
        m1, m1, 1.0,
        m1, m1, -1.0,
        
        // Middle Bottom
        -m1, -m1, 1.0,
        -m1, -m1, -1.0,
        m1, -m1, 1.0,
        m1, -m1, -1.0,
        
        // Middle Left
        -m1, m1, 1.0,
        -m1, m1, -1.0,
        -m1, -m1, 1.0,
        -m1, -m1, -1.0,
        
        // Middle Right
        m1, m1, 1.0,
        m1, m1, -1.0,
        m1, -m1, 1.0,
        m1, -m1, -1.0
        
    };
    
    GLfloat normalArray[] = {
        // Front
        0.0,  0.0,  1.0,
        0.0,  0.0,  1.0,
        0.0,  0.0,  1.0,
        0.0,  0.0,  1.0,
        0.0,  0.0,  1.0,
        0.0,  0.0,  1.0,
        0.0,  0.0,  1.0,
        0.0,  0.0,  1.0,
        0.0,  0.0,  1.0,
        0.0,  0.0,  1.0,
        0.0,  0.0,  1.0,
        0.0,  0.0,  1.0,
        
        // Back
        0.0,  0.0,  -1.0,
        0.0,  0.0,  -1.0,
        0.0,  0.0,  -1.0,
        0.0,  0.0,  -1.0,
        0.0,  0.0,  -1.0,
        0.0,  0.0,  -1.0,
        0.0,  0.0,  -1.0,
        0.0,  0.0,  -1.0,
        0.0,  0.0,  -1.0,
        0.0,  0.0,  -1.0,
        0.0,  0.0,  -1.0,
        0.0,  0.0,  -1.0,
        
        // Left
        -1.0, 0.0, 0.0,
        -1.0, 0.0, 0.0,
        -1.0, 0.0, 0.0,
        -1.0, 0.0, 0.0,
        
        // Right
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        
        // Top
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        
        // Bottom
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        
        // Middle Top
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        0.0, -1.0, 0.0,
        
        // Middle Bottom
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 1.0, 0.0,
        
        // Middle Left
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        1.0, 0.0, 0.0,
        
        // Middle Right
        -1.0, 0.0, 0.0,
        -1.0, 0.0, 0.0,
        -1.0, 0.0, 0.0,
        -1.0, 0.0, 0.0
    };
    
    GLfloat colorArray[vertexCount * 4];
    GLfloat color = 1.0;
    for (int i = 0; i < vertexCount * 4; i+=4){
        colorArray[i] = color;
        colorArray[i + 1] = color;
        colorArray[i + 2] = color;
        colorArray[i + 3] = 1.0;
    }
    
    GLushort elementArray[] = {
        // Front
        0,  3,  1,
        3,  1,  4,
        1,  4,  2,
        2,  4,  6,
        4,  6,  8,
        6,  8,  11,
        10, 8,  11,
        7,  8,  10,
        9,  7,  10,
        9,  7,  5,
        7,  5,  3,
        5,  3,  0,
        
        // Back
        12,  15,  13,
        15,  13,  16,
        13,  16,  14,
        14,  16,  18,
        16,  18,  20,
        18,  20,  23,
        22,  20,  23,
        19,  20,  22,
        21,  19,  22,
        21,  19,  17,
        19,  17,  15,
        17,  15,  12,
        
        // Left
        24, 25, 26,
        25, 26, 27,
        
        // Right
        28, 29, 30,
        29, 30, 31,
        
        // Top
        32, 33, 34,
        33, 34, 35,
        
        // Bottom
        36, 37, 38,
        37, 38, 39,
        
        // Middle Top
        40, 41, 42,
        41, 42, 43,
        
        // Middle Bottom
        44, 45, 46,
        45, 46, 47,
        
        // Middle Left
        48, 49, 50,
        49, 50, 51,
        
        // Middle Right
        52, 53, 54,
        53, 54, 55
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

void HollowCubeModel::update(GLint modelMatrixHandle) {
    velocityVector.x *= 0.99;
    velocityVector.y *= 0.99;
    velocityVector.z *= 0.99;
    
    rotateModelByVector3AndAngle(1, 0, 0, velocityVector.x);
    rotateModelByVector3AndAngle(0, 1, 0, velocityVector.y);
    
    glm::mat4 modelMatrix = BaseModel::createModelMatrix();
    
    glUniformMatrix4fv(modelMatrixHandle, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    
    mPreviousModelMatrix = modelMatrix;
}