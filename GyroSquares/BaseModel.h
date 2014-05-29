//
//  BaseModel.h
//  GyroSquares
//
//  Created by James Milton on 29/05/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//


#include "glUtil.h"
#include <string.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

enum {
    POSITION_ATTRIB_IDX,
    NORMAL_ATTRIB_IDX,
    TEXTURE_ATTRIB_IDX,
    COLOR_ATTRIB_IDX
};

class BaseModel {
public:
    BaseModel();
    ~BaseModel();
    void buildVAO();
    void drawElements();
    glm::mat4 getModelMatrix();
    void translateModelByVector3(float x, float y, float z);
    void rotateModelByVector3AndAngle(float x, float y, float z, float angle);
    
private:
    GLuint mVAO;
    glm::mat4 mModelMatrix;
    
protected:
    GLuint mNumVertcies;
	
	GLubyte *mPositions;
	GLenum mPositionType;
	GLuint mPositionSize;
	GLsizei mPositionArraySize;
	
	GLubyte *mColors;
	GLenum mColorType;
	GLuint mColorSize;
	GLsizei mColorArraySize;
    
    GLubyte *mNormals;
	GLenum mNormalType;
	GLuint mNormalSize;  
	GLsizei mNormalArraySize;
    
    GLubyte *mTextureUV;
	GLenum mTextureType;
	GLuint mTextureSize;
	GLsizei mTexureUVArraySize;
    
	GLubyte *mElements;
	GLenum mElementType;
	GLuint mNumElements;
	GLsizei mElementArraySize;
    
	GLenum mPrimType;
};