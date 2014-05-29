//
//  BaseShader.h
//  GyroSquares
//
//  Created by James Milton on 28/05/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "glUtil.h"
#include <stdio.h>
#include <string.h>
#include <iostream>

using namespace std;

class BaseShader {
public:
    BaseShader(const char* vShader, const char* fShader);
    
    GLint getUniformLocation(const char* uniformName);
    void enable();
    void disable();
    
private:
    GLuint mProgram;
};
