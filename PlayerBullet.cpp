#include "PlayerBullet.h"
#include <cassert>

void PlayerBullet::Initialize(Model* model, const Vector3& pos, const Vector3& velocity) {
	assert(model);
	model_ = model;

	textureHandle_ = TextureManager::Load("cube.jpg");

	world_.Initialize();
	world_.translation_ = pos;

	//引数で受け取った速度
	velocity_ = velocity;
}

void PlayerBullet::Update() { 
	world_.UpdateMatrix();

	world_.translation_.x += velocity_.x;
	world_.translation_.y += velocity_.y;
	world_.translation_.z += velocity_.z;

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}


}

void PlayerBullet::Draw(ViewProjection& view) { 
	model_->Draw(world_,view,textureHandle_);
}

