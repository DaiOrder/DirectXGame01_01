#pragma once
/// <summary>
/// 4x4行列
/// </summary>
struct Matrix4x4 final {
	float m[4][4];
};

struct Vector3 {
	float x, y, z;
};

//スケーリング行列
Matrix4x4 MakeScaleMatrix(const Vector3 scale);

//回転行列
Matrix4x4 MakeRotationXMatrix(float theta);
Matrix4x4 MakeRotationYMatrix(float theta);
Matrix4x4 MakeRotationZMatrix(float theta);

//平行移動行列
Matrix4x4 MakeTransfomelateMatrix(Vector3 trans);

Matrix4x4 cperator *= (Matrix4x4& m1, const Matrix4x4& m2){

};


 Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rot, const Vector3& tanslate);

