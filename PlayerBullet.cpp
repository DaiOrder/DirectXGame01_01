#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& pos, const Vector3& velocity) {
	assert(model);
	model_ = model;

	textureHandle_ = TextureManager::Load("cube.jpg");

	world_.Initialize();
	world_.translation_ = pos;

	// 引数で受け取った速度
	velocity_ = velocity;
}

void PlayerBullet::Update() { 
	world_.UpdateMatrix();

	world_.translation_.x += velocity_.x;
	world_.translation_.y += velocity_.y;
	world_.translation_.z += velocity_.z;

	// 時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

}

void PlayerBullet::Draw(ViewProjection& view) { 
	model_->Draw(world_,view,textureHandle_); }

//当たり判定の自機弾のデスフラグを立てる
void PlayerBullet::OnCollision() { 
	isDead_ = true; 

}

Vector3 PlayerBullet::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得 (ワールド座標)
	worldPos.x = world_.translation_.x;
	worldPos.y = world_.translation_.y;
	worldPos.z = world_.translation_.z;

	return worldPos;
}

