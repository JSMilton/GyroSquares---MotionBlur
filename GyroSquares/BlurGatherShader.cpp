//
//  BlurGatherShader.cpp
//  GyroSquares
//
//  Created by James Milton on 02/06/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "BlurGatherShader.h"

BlurGatherShader::BlurGatherShader() : BaseShader("blurGather", "blurGather") {
    mColorTextureHandle = getUniformLocation("uColorTexture");
    mVelocityTextureHandle = getUniformLocation("uVelocityTexture");
    //mDepthTextureHandle = getUniformLocation("uDepthTexture");
}