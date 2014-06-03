//
//  BlurTileMaxShader.h
//  GyroSquares
//
//  Created by James Milton on 03/06/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//


#include "BaseShader.h"

class BlurTileMaxShader : public BaseShader {
public:
    BlurTileMaxShader();
    GLint mInputTextureHandle;
    GLint mKHandle;
};