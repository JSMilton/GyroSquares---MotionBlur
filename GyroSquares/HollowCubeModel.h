//
//  HollowCubeModel.h
//  GyroSquares
//
//  Created by James Milton on 02/06/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "BaseModel.h"

class HollowCubeModel : public BaseModel  {
public:
    HollowCubeModel();
    void update(GLint modelMatrixHandle);
    glm::vec3 velocityVector;
};