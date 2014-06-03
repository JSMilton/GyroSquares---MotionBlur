//
//  BlurNeighbourMaxShader.cpp
//  GyroSquares
//
//  Created by James Milton on 03/06/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "BlurNeighbourMaxShader.h"

BlurNeighbourMaxShader::BlurNeighbourMaxShader() : BaseShader("blurNeighbourMax", "blurNeighbourMax") {
    mInputTextureHandle = getUniformLocation("uInputTexture");
}