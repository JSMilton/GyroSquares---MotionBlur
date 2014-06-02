//
//  ScreenQuadModel.cpp
//  GyroSquares
//
//  Created by James Milton on 02/06/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "ScreenQuadModel.h"

ScreenQuadModel::ScreenQuadModel() : BaseModel() {
    const int vertexCount = 6;
    // x,y vertex positions
    float positionArray[] = {
        -1.0, -1.0, 0.0,
        1.0, -1.0, 0.0,
        1.0,  1.0, 0.0,
        1.0,  1.0, 0.0,
        -1.0,  1.0, 0.0,
        -1.0, -1.0, 0.0
    };
    // per-vertex texture coordinates
    float textureUVArray[] = {
        0.0, 0.0,
        1.0, 0.0,
        1.0, 1.0,
        1.0, 1.0,
        0.0, 1.0,
        0.0, 0.0
    };

    mPositionSize = 3;
    mPositionArraySize = sizeof(positionArray);
    mPositionType = GL_FLOAT;
    mPositions = (GLubyte *)malloc(mPositionArraySize);
    memcpy(mPositions, positionArray, mPositionArraySize);
    
    mTextureSize = 2;
    mTexureUVArraySize = sizeof(textureUVArray);
    mTextureType = GL_FLOAT;
    mTextureUV = (GLubyte *)malloc(mTexureUVArraySize);
    memcpy(mTextureUV, textureUVArray, mTexureUVArraySize);
    
    mPrimType = GL_TRIANGLES;
    
    mNumVertcies = vertexCount;
}
