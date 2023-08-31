#include "Effect.h"
#include "Enemy.h"

void Effect::Initialize(Model* model, const Vector3& pos, const Vector3& ppos) {
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;

	ePos_ = ppos;
}

void Effect::Update() { 
	//Vector3 enemyPos = ePos_;

	worldTransform_.translation_.x += ePos_.x;
	worldTransform_.translation_.y += ePos_.y;
	worldTransform_.translation_.z += ePos_.z;

}

void Effect::SetWorldPosition(const Vector3& pos) { 
	//Vector3 GetEnemyPos;

	ePos_.x += pos.x;
	ePos_.y += pos.y;
	ePos_.z += pos.z;

}

// 描画
void Effect::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);

}