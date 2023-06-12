#include "Enemy.h"
#include <cassert>
#include <ImGuiManager.h>

// 初期化
void Enemy::Initialize(Model* model, const Vector3& pos) {
	assert(model);
	model_ = model;

	textureHandle_ = TextureManager::Load("sample.png");

	world_.Initialize();
	world_.translation_ = pos;
}

// アプデ
void Enemy::Update() {
	world_.UpdateMatrix();

	const float kEnemySpeed = -0.2f;
	velocity_ = {0, 0, kEnemySpeed};

	switch (phase_) { 
	case Phase::Approach:
	default:
		// 移動 (ベクトルを加算)
		world_.translation_.x += velocity_.x;
		world_.translation_.y += velocity_.y;
		world_.translation_.z += velocity_.z;

		if (world_.translation_.z < -10.0f) {
			phase_ = Phase::Leave;
		}
		break;

	case Phase::Leave:
		velocity_.x = 0.2f;
		velocity_.y = 0.1f;

		// 移動 (ベクトルを加算)
		world_.translation_.x += velocity_.x;
		world_.translation_.y += velocity_.y;
		world_.translation_.z += velocity_.z;
		break;
	}

	ImGui::Begin("Window");
	ImGui::DragFloat3("world_.translation_.z", &world_.translation_.x, 0.01f);
	ImGui::End();
}

// 描画
void Enemy::Draw(ViewProjection& view) { 
	model_->Draw(world_, view, textureHandle_);
	
}