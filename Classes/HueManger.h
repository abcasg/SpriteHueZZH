/********************************
File name: HueManger
Author: Zihu Zhang
Version: 1.0
Date: 2015.10.10
Description: 精灵色相设置
********************************/

#ifndef _HUEMANGER_H_
#define _HUEMANGER_H_

#include "cocos2d.h"

USING_NS_CC;



class HueManger : public Ref
{
public:

	GLProgramState* getHueGLProgramState(float color = 360); // 0~360

	static HueManger* create();

private:

	HueManger();

	~HueManger();

	bool init();

	void matrixMult(float a[3][3], float b[3][3], float c[3][3]);
	void zRotateMat(float mat[3][3], float rs, float rc);
	void yRotateMat(float mat[3][3], float rs, float rc);
	void xRotateMat(float mat[3][3], float rs, float rc);
	void hueMatrix(GLfloat mat[3][3], float angle);
	void hueUniformCallback(cocos2d::GLProgram *p, cocos2d::Uniform *u);
	const GLchar * colorRotationShaderBody();

private:
	GLfloat _mat[3][3];
};

#endif // _HUEMANGER_H_