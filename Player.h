﻿#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <cassert>

#include <list>

class Player {
public:
	//初期化
	void Initialize(Model* model, uint32_t textureHandle, Vector3& position);

	//更新
	void Update();

	//描画
	void Draw(ViewProjection& viewProjection);

	//弾発射
	void Attack();

	//ワールド座標を取得
	Vector3 GetWorldPosition();

	~Player();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();
	
	// 弾リストを取得
	const std::list<PlayerBullet*> GetBullets() const { return bullets_; }

	// レールカメラとのペアレント
	void SetParent(const WorldTransform* parent);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	Model* model3DReticle = nullptr;

	// 自機テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//旋回
	WorldTransform rotation_;

	// 2-2～
	// キーボード入力
	Input* input_ = nullptr;

	PlayerBullet* bullet_ = nullptr;

	std::list<PlayerBullet*> bullets_;

	// 3Dレティクル用ワールドトランスフォーム
	WorldTransform worldTransform3DReticle_;

};

// 正規化
inline Vector3 Normalize(const Vector3& v) {
	Vector3 result;
	Vector3 math;

	math.x = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
	result.x = v.x / math.x;
	result.y = v.y / math.x;
	result.z = v.z / math.x;

	return result;
}

// 座標変換
inline Vector3 Transform(const Vector3& Vector, const Matrix4x4& matrix) {
	Vector3 result{
	    Vector.x * matrix.m[0][0] + Vector.y * matrix.m[1][0] + Vector.z * matrix.m[2][0],
	    Vector.x * matrix.m[0][1] + Vector.y * matrix.m[1][1] + Vector.z * matrix.m[2][1],
	    Vector.x * matrix.m[0][2] + Vector.y * matrix.m[1][2] + Vector.z * matrix.m[2][2]
	};

	return result;
}

// 3次元回転行列
inline Vector3 Multiply2(float scalar, Vector3 v) {

	Vector3 result;
	result.x = scalar * v.x;
	result.y = scalar * v.y;
	result.z = scalar * v.z;

	return result;
}

// 加算
inline Vector3 Add(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;
}

// 減算
inline Vector3 Subtract(const Vector3& v1, const Vector3& v2) {
	Vector3 result;
	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;

	return result;
}

// 内積
inline float Dot(const Vector3& v1, const Vector3& v2) {
	float result = 0.0f;

	result = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;

	return result;
}
