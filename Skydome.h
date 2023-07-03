#pragma once
#include "WorldTransform.h"
#include "Model.h"

class Skydome {
public:
	//初期化
	void Initialize(Model* model/*, uint32_t textureHandle*/);

	//更新
	void Update();

	//描画
	void Draw(ViewProjection& viewProjection);

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//モデル
	Model* model_ = nullptr;

};
