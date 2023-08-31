#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Enemy.h"


class Effect {
public:
	// 初期化
	void Initialize(Model* model, const Vector3& pos, const Vector3& ppos);

	// 更新
	void Update(/*const Vector3& pos*/);

	// 描画
	void Draw(ViewProjection& viewProjection);

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	void SetWorldPosition(const Vector3& pos);

private:

	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	Vector3 ePos_;

};
