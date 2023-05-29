#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class Enemy {
public:
	//初期化
	void Initialize(Model* model, const Vector3& pos);

	//アプデ
	void Update();

	//描画
	void Draw(ViewProjection& view);

	~Enemy();

private:
	//ワールドトランスフォーム
	WorldTransform world_;

	//モデル
	Model* model_;

	//リソース
	uint32_t textureHandle_;

	// 速度
	Vector3 velocity_;

};