//
//  BlurGatherShader.h
//  GyroSquares
//
//  Created by James Milton on 02/06/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "BaseShader.h"

class BlurGatherShader : public BaseShader {
public:
    BlurGatherShader();
    GLint mColorTextureHandle;
    GLint mVelocityTextureHandle;
    GLint mNeighbourMaxTextureHandle;
    GLint mDepthTextureHandle;
};
