#pragma once

#include "WorldTransform.h"
#include "Model.h"
//#include"GameScene.h"

class Player {
public:
	//初期化

	void Initialize(Model* model, uint32_t textureHandle);

	//更新
	void Update();

	//描画
	void Draw(ViewProjection& viewProjection);

	private:
		//ワールド変換データ
		WorldTransform worldTransform_;

		ViewProjection viewProjection_;

		//モデル
	    Model* model_ = nullptr;

		//テクスチャハンドル
	    uint32_t textureHandle_ = 0u;


};
