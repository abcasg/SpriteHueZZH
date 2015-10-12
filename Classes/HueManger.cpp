#include "HueManger.h"


const GLchar * HueManger::colorRotationShaderBody()
{
	return
		"                                                               \n\
							    #ifdef GL_ES                                                    \n\
																				precision mediump float;                                        \n\
																																							    #endif                                                          \n\
																																																																						\n\
																																																																																																																varying vec2 v_texCoord;                                        \n\
																																																																																																																																																																								uniform mat3 u_hue;                                             \n\
																																																																																																																																																																																																																																																\n\
																																																																																																																																																																																																																																																																																																																																										void main()                                                     \n\
																																																																																																																																																																																																																																																																																																																																																																																																																																																								{                                                               \n\
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																											    vec4 pixColor = texture2D(CC_Texture0, v_texCoord);             \n\
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																								vec3 rgbColor = u_hue * pixColor.rgb;                           \n\
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																														gl_FragColor = vec4(rgbColor, pixColor.a );                     \n\
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																}                                                               \n\
																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																														    ";
}

HueManger::HueManger()
{

}

HueManger::~HueManger()
{

}

HueManger* HueManger::create()
{
	HueManger* hueManger = new HueManger();

	if (hueManger && hueManger->init())
	{
		hueManger->autorelease();
		hueManger->retain();
		return hueManger;
	}
	CC_SAFE_DELETE(hueManger);

	return NULL;
}

bool HueManger::init()
{
	return true;
}


GLProgramState* HueManger::getHueGLProgramState(float color)
{
	auto glprogram = GLProgramCache::getInstance()->getGLProgram("hue_program");
	if (!glprogram)
	{
		glprogram = GLProgram::createWithByteArrays(ccPositionTextureColor_noMVP_vert, colorRotationShaderBody());
		GLProgramCache::getInstance()->addGLProgram(glprogram, "hue_program");
	}

	color = color / 360;
	hueMatrix(_mat, 2 * M_PI* color);

	auto glprogramstate = GLProgramState::create(glprogram);
	glprogramstate->setUniformCallback("u_hue", std::bind(&HueManger::hueUniformCallback, this, std::placeholders::_1, std::placeholders::_2));

	return glprogramstate;
}

void HueManger::hueUniformCallback(cocos2d::GLProgram *p, cocos2d::Uniform *u)
{
	glUniformMatrix3fv(u->location, 1, GL_FALSE, (GLfloat*)&_mat);
}


void HueManger::hueMatrix(GLfloat mat[3][3], float angle)
{
#define SQRT_2      sqrt(2.0)
#define SQRT_3      sqrt(3.0)

	float mag, rot[3][3];
	float xrs, xrc;
	float yrs, yrc;
	float zrs, zrc;

	// Rotate the grey vector into positive Z
	mag = SQRT_2;
	xrs = 1.0 / mag;
	xrc = 1.0 / mag;
	xRotateMat(mat, xrs, xrc);
	mag = SQRT_3;
	yrs = -1.0 / mag;
	yrc = SQRT_2 / mag;
	yRotateMat(rot, yrs, yrc);
	matrixMult(rot, mat, mat);

	// Rotate the hue
	zrs = sin(angle);
	zrc = cos(angle);
	zRotateMat(rot, zrs, zrc);
	matrixMult(rot, mat, mat);

	// Rotate the grey vector back into place
	yRotateMat(rot, -yrs, yrc);
	matrixMult(rot, mat, mat);
	xRotateMat(rot, -xrs, xrc);
	matrixMult(rot, mat, mat);
}

void HueManger::xRotateMat(float mat[3][3], float rs, float rc)
{
	mat[0][0] = 1.0;
	mat[0][1] = 0.0;
	mat[0][2] = 0.0;

	mat[1][0] = 0.0;
	mat[1][1] = rc;
	mat[1][2] = rs;

	mat[2][0] = 0.0;
	mat[2][1] = -rs;
	mat[2][2] = rc;
}

void HueManger::yRotateMat(float mat[3][3], float rs, float rc)
{
	mat[0][0] = rc;
	mat[0][1] = 0.0;
	mat[0][2] = -rs;

	mat[1][0] = 0.0;
	mat[1][1] = 1.0;
	mat[1][2] = 0.0;

	mat[2][0] = rs;
	mat[2][1] = 0.0;
	mat[2][2] = rc;
}


void HueManger::zRotateMat(float mat[3][3], float rs, float rc)
{
	mat[0][0] = rc;
	mat[0][1] = rs;
	mat[0][2] = 0.0;

	mat[1][0] = -rs;
	mat[1][1] = rc;
	mat[1][2] = 0.0;

	mat[2][0] = 0.0;
	mat[2][1] = 0.0;
	mat[2][2] = 1.0;
}
void HueManger::matrixMult(float a[3][3], float b[3][3], float c[3][3])
{
	int x, y;
	float temp[3][3];

	for (y = 0; y < 3; y++) {
		for (x = 0; x < 3; x++) {
			temp[y][x] = b[y][0] * a[0][x] + b[y][1] * a[1][x] + b[y][2] * a[2][x];
		}
	}
	for (y = 0; y < 3; y++) {
		for (x = 0; x < 3; x++) {
			c[y][x] = temp[y][x];
		}
	}
}