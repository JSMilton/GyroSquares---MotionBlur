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

SquareModel *loadSquare()
{
    //
    // SquareModels are essentially hollow cuboids, like a 3D picture frame.
    // I'm using triangles, which means 12 vertices for each face, 24 in total.
    //
    
    const int vertexCount = 24;
    
    GLfloat m1;
    m1 = 0.5;
    
    GLfloat positionArray[] = {
       // x     y     z
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
         1.0,  1.0, 0.5,
         0.0,  1.0, 0.5,
        -1.0,  1.0, 0.5,
         m1,   m1,  0.5,
        -m1,   m1,  0.5,
         1.0,  0.0, 0.5,
        -1.0,  0.0, 0.5,
         m1,  -m1,  0.5,
        -m1,  -m1,  0.5,
         1.0, -1.0, 0.5,
         0.0, -1.0, 0.5,
        -1.0, -1.0, 0.5
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
        
        // Top
        0,  1,  12,
        12, 1,  14,
        1,  14, 2,
        
        // Left
        0, 12, 5,
        12, 21, 5,
        21, 5, 9,
        
        // Right
        2, 14, 6,
        14, 23, 6,
        6, 11, 23,
        
        // Bottom
        9, 21, 10,
        21, 10, 23,
        10, 11, 23,
        
        // Inside Top
        3, 15, 4,
        15, 4, 16,
        
        // Inside Left
        3, 15, 7,
        3, 7, 19,
        
        // Inside Right
        4, 8, 20,
        20, 16, 4,
        
        // Inside Bottom
        7, 8, 20,
        20, 7, 19
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

SquareModel *loadBasicSquare()
{
    const int vertexCount = 4;
    
    GLfloat m1;
    m1 = 0.75;
    
    GLfloat positionArray[] = {
        // x     y     z
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f
    };
    
    GLfloat colorArray[vertexCount * 4];
    GLfloat color = 1.0f;
    for (int i = 0; i < vertexCount * 4; i+=4){
        colorArray[i] = color;
        colorArray[i + 1] = color;
        colorArray[i + 2] = color;
        colorArray[i + 3] = 1.0f;
    }
    
    GLuint elementArray[] = {
        // Front
        0,  1,  2,
        1,  2,  3
    };
    
    SquareModel *newModel = calloc(sizeof(SquareModel), 1);
    
    // check for memory error
    if (newModel == NULL){
        return NULL;
    }
    
    newModel->positionSize = 3;
    newModel->positionArraySize = sizeof(positionArray);
    newModel->positionType = GL_FLOAT;
    newModel->positions = malloc(newModel->positionArraySize);
    memcpy(newModel->positions, positionArray, newModel->positionArraySize);
    
    newModel->colorSize = 4;
    newModel->colorArraySize = sizeof(colorArray);
    newModel->colorType = GL_FLOAT;
    newModel->colors = malloc(newModel->colorArraySize);
    memcpy(newModel->colors, colorArray, sizeof(newModel->colorArraySize));
    
    newModel->primType = GL_TRIANGLES;
    
    newModel->numElements = sizeof(elementArray) / sizeof(GLuint);
    newModel->elementType = GL_UNSIGNED_INT;
    newModel->elementArraySize = sizeof(elementArray);
    newModel->elements = malloc(newModel->elementArraySize);
    memcpy(newModel->elements, elementArray, sizeof(newModel->elementArraySize));
    
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

