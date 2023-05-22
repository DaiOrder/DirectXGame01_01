#pragma once

#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "PlayerBullet.h"

#include <list>

class Player {
public:
	//初期化
	void Initialize(Model* model, uint32_t textureHandle);

	//更新
	void Update();

	//描画
	void Draw(ViewProjection& viewProjection);

	//弾発射
	void Attack();

	~Player();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//旋回
	WorldTransform rotation_;

	// 2-2～
	// キーボード入力
	Input* input_ = nullptr;

	PlayerBullet* bullet_ = nullptr;

	std::list<PlayerBullet*> bullets_;

};
