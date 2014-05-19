//
//  squareUtil.c
//  GyroSquares
//
//  Created by James Milton on 25/04/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "squareUtil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

SquareModel *loadHollowCuboid()
{
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
    
    SquareModel *newModel = (SquareModel *)calloc(sizeof(SquareModel), 1);
    
    // check for memory error
    if (newModel == NULL){
        return NULL;
    }
    
    newModel->positionSize = 3;
    newModel->positionArraySize = sizeof(positionArray);
    newModel->positionType = GL_FLOAT;
    newModel->positions = (GLubyte *)malloc(newModel->positionArraySize);
    memcpy(newModel->positions, positionArray, newModel->positionArraySize);
    
    newModel->normalSize = 3;
    newModel->normalArraySize = sizeof(normalArray);
    newModel->normalType = GL_FLOAT;
    newModel->normals = (GLubyte *)malloc(newModel->normalArraySize);
    memcpy(newModel->normals, normalArray, newModel->normalArraySize);
    
    newModel->colorSize = 4;
    newModel->colorArraySize = sizeof(colorArray);
    newModel->colorType = GL_FLOAT;
    newModel->colors = (GLubyte *)malloc(newModel->colorArraySize);
    memcpy(newModel->colors, colorArray, newModel->colorArraySize);
    
    newModel->primType = GL_TRIANGLES;
    
    newModel->numElements = sizeof(elementArray) / sizeof(GLushort);
    newModel->elementType = GL_UNSIGNED_SHORT;
    newModel->elementArraySize = sizeof(elementArray);
    newModel->elements = (GLubyte *)malloc(newModel->elementArraySize);
    memcpy(newModel->elements, elementArray, newModel->elementArraySize);
    
    newModel->numVertcies = vertexCount;
    
    return newModel;
}

SquareModel *loadCube()
{
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
    
    SquareModel *newModel = (SquareModel *)calloc(sizeof(SquareModel), 1);
    
    // check for memory error
    if (newModel == NULL){
        return NULL;
    }
    
    newModel->positionSize = 3;
    newModel->positionArraySize = sizeof(positionArray);
    newModel->positionType = GL_FLOAT;
    newModel->positions = (GLubyte *)malloc(newModel->positionArraySize);
    memcpy(newModel->positions, positionArray, newModel->positionArraySize);
    
    newModel->normalSize = 3;
    newModel->normalArraySize = sizeof(normalArray);
    newModel->normalType = GL_FLOAT;
    newModel->normals = (GLubyte *)malloc(newModel->normalArraySize);
    memcpy(newModel->normals, normalArray, newModel->normalArraySize);
    
    newModel->colorSize = 4;
    newModel->colorArraySize = sizeof(colorArray);
    newModel->colorType = GL_FLOAT;
    newModel->colors = (GLubyte *)malloc(newModel->colorArraySize);
    memcpy(newModel->colors, colorArray, newModel->colorArraySize);
    
    newModel->primType = GL_TRIANGLES;
    
    newModel->numElements = sizeof(elementArray) / sizeof(GLushort);
    newModel->elementType = GL_UNSIGNED_SHORT;
    newModel->elementArraySize = sizeof(elementArray);
    newModel->elements = (GLubyte *)malloc(newModel->elementArraySize);
    memcpy(newModel->elements, elementArray, newModel->elementArraySize);
    
    newModel->numVertcies = vertexCount;
    
    return newModel;
}

void destroySquareModel(SquareModel *model)
{
    if (model == NULL){
        return;
    }
    
    free(model->elements);
    free(model->colors);
    free(model->positions);
    
    free(model);
}

