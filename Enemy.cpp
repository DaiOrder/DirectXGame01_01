#include "Enemy.h"
#include <cassert>

// 初期化
void Enemy::Initialize(Model* model, const Vector3& pos) {
	assert(model);
	model_ = model;

	textureHandle_ = TextureManager::Load("sample.png");

	world_.Initialize();
	world_.translation_ = pos;

	// 引数で受け取った速度
	//velocity_ = velocity;
}

Enemy::~Enemy() {

}

// アプデ
void Enemy::Update() {
	world_.UpdateMatrix();

	const float kEnemySpeed = -0.2f;
	velocity_ = {0, 0, kEnemySpeed};

	world_.translation_.x += velocity_.x;
	world_.translation_.y += velocity_.y;
	world_.translation_.z += velocity_.z;
}

// 描画
void Enemy::Draw(ViewProjection& view) { model_->Draw(world_, view, textureHandle_); }