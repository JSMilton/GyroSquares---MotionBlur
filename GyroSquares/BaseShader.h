//
//  BaseShader.h
//  GyroSquares
//
//  Created by James Milton on 28/05/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#ifndef BASE_SHADER_H
#define BASE_SHADER_H

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

#endif // BASE_SHADER_H
