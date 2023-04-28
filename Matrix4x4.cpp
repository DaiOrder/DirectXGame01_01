#include "Matrix4x4.h"
#include <cmath>


//スケーリング行列
Matrix4x4 MakeScaleMatrix(const Vector3 scale) { 
	Matrix4x4 result = {
		scale.x, 0, 0, 0,
		0, scale.y, 0, 0,
		0, 0, scale.z, 0,
		0, 0, 0, 1
	};


};

// 回転行列
Matrix4x4 MakeRotationXMatrix(float theta) {
	float sin = std::sin(theta);
	float cos = std::cos(theta);

	Matrix4x4 result = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f, cos,sin,0.0f,
		0.0f,-sin,cos,0.0f,
		0.0f,0.0f,0.0f,1.0f
	};

	return result;
}

Matrix4x4 MakeRotationYMatrix(float theta) {
	float sin = std::sin(theta);
	float cos = std::cos(theta);

	Matrix4x4 result = {
		cos, 0.0f, -sin, 0.0f,
		0.0f, 1.0f,  0.0f,  0.0f,
	    sin, -0.0f, cos,  0.0f, 
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return result;
}

Matrix4x4 MakeRotationZMatrix(float theta) {
	float sin = std::sin(theta);
	float cos = std::cos(theta);

	Matrix4x4 result = {
		cos , sin, 0.0f, 0.0f,
		-sin, cos,  0.0f,  0.0f,
	    0.0f, 0.0f, 1.0f,  0.0f,
		0.0f, 0.0f, 0.0f,  1.0f
	};

	return result;
}


//平行移動行列
Matrix4x4 MakeTransfomelateMatrix(Vector3 trans) {
	Matrix4x4 result = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
	    0.0f, 0.0f, 1.0f, 0.0f,
		trans.x, trans.y, trans.z, 1.0
	};
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& tanslate) {

	Matrix4x4 matScale = MakeScaleMatrix(scale);

	Matrix4x4 matRotX = MakeRotationXMatrix(rot.x);
	Matrix4x4 matRotY = MakeRotationYMatrix(rot.y);
	Matrix4x4 matRotZ = MakeRotationZMatrix(rot.z);

	Matrix4x4 matRot = matRotX * matRotY * matRotZ;

	Matrix4x4 matTrans = MakeTransfomelateMatrix(trans);

	Matrix4x4;

	return Matrix4x4();
};