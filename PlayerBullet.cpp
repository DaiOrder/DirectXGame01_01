#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& pos, const Vector3& velocity) {
	assert(model);
	model_ = model;

	textureHandle_ = TextureManager::Load("cube.jpg");

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;

	// 引数で受け取った速度
	velocity_ = velocity;
}

void PlayerBullet::Update() { 
	worldTransform_.UpdateMatrix();

	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void PlayerBullet::Draw(ViewProjection& view) { 
	model_->Draw(worldTransform_,view,textureHandle_); }

//当たり判定の自機弾のデスフラグを立てる
void PlayerBullet::OnCollision() { 
	isDead_ = true; 

}

Vector3 PlayerBullet::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得 (ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

