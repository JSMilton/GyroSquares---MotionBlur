//
//  BaseModel.h
//  GyroSquares
//
//  Created by James Milton on 29/05/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "glUtil.h"
#include "glm/glm.hpp"

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
    glm::mat4 setModelMatrix(glm::mat4 newMatrix);
    
private:
    glm::mat4 mModelMatrix;
    GLuint mVAO;
    
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