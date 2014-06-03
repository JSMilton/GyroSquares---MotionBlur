//
//  BaseModel.cpp
//  GyroSquares
//
//  Created by James Milton on 29/05/2014.
//  Copyright (c) 2014 James Milton. All rights reserved.
//

#include "BaseModel.h"

BaseModel::BaseModel() {
    mTranslationMatrix = glm::mat4(1.0);
    mScaleMatrix = glm::mat4(1.0);
    mRotationMatrix = glm::mat4(1.0);
    mVAO = 0;
    
    mNumVertcies = 0;
    mPositions = NULL;
    mPositionType = 0;
    mPositionSize = 0;
    mPositionArraySize = 0;
	
    mColors = NULL;
    mColorType = 0;
    mColorSize = 0;
    mColorArraySize = 0;
    
    mNormals = NULL;
    mNormalType = 0;
    mNormalSize = 0;
    mNormalArraySize = 0;
    
    mTextureUV = NULL;
    mTextureType = 0;
    mTextureSize = 0;
    mTexureUVArraySize = 0;
    
    mElements = NULL;
    mElementType = 0;
    mNumElements = 0;
    mElementArraySize = 0;
    
    mPrimType = GL_TRIANGLES;
}

BaseModel::~BaseModel() {
    free(mElements);
    free(mColors);
    free(mPositions);
    free(mNormals);
    free(mTextureUV);
}

void BaseModel::drawElements() {
    glBindVertexArray(mVAO);
    glDrawElements(mPrimType, mNumElements, mElementType, 0);
    glBindVertexArray(0);
}

void BaseModel::drawArrays() {
    glBindVertexArray(mVAO);
    glDrawArrays(mPrimType, 0, mNumVertcies);
    glBindVertexArray(0);
}

void BaseModel::buildVAO() {
    glGenVertexArrays(1, &mVAO);
    glBindVertexArray(mVAO);
    
    GLuint posBufferName;
    
    // Create a vertex buffer object (VBO) to store positions
    glGenBuffers(1, &posBufferName);
    glBindBuffer(GL_ARRAY_BUFFER, posBufferName);
    
    // Allocate and load position data into the VBO
    glBufferData(GL_ARRAY_BUFFER, mPositionArraySize, mPositions, GL_STATIC_DRAW);
    
    // Enable the position attribute for this VAO
    glEnableVertexAttribArray(POSITION_ATTRIB_IDX);
    
    // Get the size of the position type so we can set the stride properly
    GLsizei posTypeSize = GetGLTypeSize(mPositionType);
    
    glVertexAttribPointer(POSITION_ATTRIB_IDX,		// What attibute index will this array feed in the vertex shader (see buildProgram)
                          mPositionSize,	// How many elements are there per position?
                          mPositionType,	// What is the type of this data?
                          GL_FALSE,				// Do we want to normalize this data (0-1 range for fixed-pont types)
                          mPositionSize*posTypeSize, // What is the stride (i.e. bytes between positions)?
                          0);	// What is the offset in the VBO to the position data?
    
    if (mNormalSize > 0){
        GLuint normalBufferName;
        // Create a vertex buffer object (VBO) to store positions
        glGenBuffers(1, &normalBufferName);
        glBindBuffer(GL_ARRAY_BUFFER, normalBufferName);
        
        // Allocate and load position data into the VBO
        glBufferData(GL_ARRAY_BUFFER, mNormalArraySize, mNormals, GL_STATIC_DRAW);
        
        // Enable the position attribute for this VAO
        glEnableVertexAttribArray(NORMAL_ATTRIB_IDX);
        
        // Get the size of the position type so we can set the stride properly
        posTypeSize = GetGLTypeSize(mNormalType);
        
        glVertexAttribPointer(NORMAL_ATTRIB_IDX,		// What attibute index will this array feed in the vertex shader (see buildProgram)
                              mNormalSize,	// How many elements are there per position?
                              mNormalType,	// What is the type of this data?
                              GL_FALSE,				// Do we want to normalize this data (0-1 range for fixed-pont types)
                              mNormalSize*posTypeSize, // What is the stride (i.e. bytes between positions)?
                              0);	// What is the offset in the VBO to the position data?
    }
    
    if (mColorSize > 0){
        GLuint colorBufferName;
        // Create a vertex buffer object (VBO) to store positions
        glGenBuffers(1, &colorBufferName);
        glBindBuffer(GL_ARRAY_BUFFER, colorBufferName);
        
        // Allocate and load position data into the VBO
        glBufferData(GL_ARRAY_BUFFER, mColorArraySize, mColors, GL_STATIC_DRAW);
        
        // Enable the position attribute for this VAO
        glEnableVertexAttribArray(COLOR_ATTRIB_IDX);
        
        // Get the size of the position type so we can set the stride properly
        posTypeSize = GetGLTypeSize(mColorType);
        
        glVertexAttribPointer(COLOR_ATTRIB_IDX,		// What attibute index will this array feed in the vertex shader (see buildProgram)
                              mColorSize,	// How many elements are there per position?
                              mColorType,	// What is the type of this data?
                              GL_FALSE,				// Do we want to colorize this data (0-1 range for fixed-pont types)
                              mColorSize*posTypeSize, // What is the stride (i.e. bytes between positions)?
                              0);	// What is the offset in the VBO to the position data?
    }
    
    if (mTextureSize > 0){
        GLuint textureBufferName;
        // Create a vertex buffer object (VBO) to store positions
        glGenBuffers(1, &textureBufferName);
        glBindBuffer(GL_ARRAY_BUFFER, textureBufferName);
        
        // Allocate and load position data into the VBO
        glBufferData(GL_ARRAY_BUFFER, mTexureUVArraySize, mTextureUV, GL_STATIC_DRAW);
        
        // Enable the position attribute for this VAO
        glEnableVertexAttribArray(TEXTURE_ATTRIB_IDX);
        
        // Get the size of the position type so we can set the stride properly
        posTypeSize = GetGLTypeSize(mTextureType);
        
        glVertexAttribPointer(TEXTURE_ATTRIB_IDX,		// What attibute index will this array feed in the vertex shader (see buildProgram)
                              mTextureSize,	// How many elements are there per position?
                              mTextureType,	// What is the type of this data?
                              GL_FALSE,				// Do we want to colorize this data (0-1 range for fixed-pont types)
                              mTextureSize*posTypeSize, // What is the stride (i.e. bytes between positions)?
                              0);	// What is the offset in the VBO to the position data?
    }
    
    if (mElementArraySize > 0){
        GLuint elementBufferName;
        // Create a VBO to vertex array elements
        // This also attaches the element array buffer to the VAO
        glGenBuffers(1, &elementBufferName);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBufferName);
        
        // Allocate and load vertex array element data into VBO
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mElementArraySize, mElements, GL_STATIC_DRAW);
    }
}

void BaseModel::translateModelByVector3(float x, float y, float z) {
    mTranslationMatrix = glm::translate(mTranslationMatrix, glm::vec3(x,y,z));
}

void BaseModel::rotateModelByVector3AndAngle(float x, float y, float z, float angle) {
    mRotationMatrix = glm::rotate(mRotationMatrix, angle, glm::vec3(x,y,z));
}

void BaseModel::scaleModelByVector3(float x, float y, float z) {
    mScaleMatrix = glm::scale(mScaleMatrix, glm::vec3(x,y,z));
}

glm::mat4 BaseModel::getPreviousModelMatrix() {
    return mPreviousModelMatrix;
}

glm::mat4 BaseModel::getModelMatrix() {
    return createModelMatrix();
}

glm::mat4 BaseModel::createModelMatrix() {
    glm::mat4 modelMatrix = mTranslationMatrix * mRotationMatrix * mScaleMatrix;
    return modelMatrix;
}
