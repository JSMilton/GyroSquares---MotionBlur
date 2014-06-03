//
//  BlurTileMaxShader.cpp
//  GyroSquares
//
//  Created by James Milton on 03/06/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "BlurTileMaxShader.h"

BlurTileMaxShader::BlurTileMaxShader() : BaseShader("blurTileMax", "blurTileMax") {
    mInputTextureHandle = getUniformLocation("uInputTexture");
    mKHandle = getUniformLocation("uK");
}