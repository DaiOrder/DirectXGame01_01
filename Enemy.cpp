#include "Enemy.h"
#include <cassert>
#include <ImGuiManager.h>
#include "EnemyBullet.h"

// 初期化
void Enemy::Initialize(Model* model, const Vector3& pos) {
	assert(model);
	model_ = model;

	textureHandle_ = TextureManager::Load("sample.png");

	world_.Initialize();
	world_.translation_ = pos;

	PhaseInitialize();

}

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		if (bullet->IsDead()) {
			delete bullet;
		}
	}
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
	// デスフラグの立った弾を削除
		bullets_.remove_if([](EnemyBullet* bullet) {
			if (bullet->IsDead()) {
				delete bullet;
				return true;
			}
			return false;
		});

		Timer--;
		if (Timer == 0) {
			// 弾発射
			Fire();

			Timer = kFireInterval;
	
		}

	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	
}

void Enemy::PhaseInitialize() {
	//発射タイマーの初期化
	Timer = kFireInterval;

}

void Enemy::Fire() {
	Vector3 Velocity(0, 0.01f, -0.8f);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, world_.translation_, Velocity);

	// 弾を登録する
	bullets_.push_back(newBullet);

}

// 描画
void Enemy::Draw(ViewProjection& view) { 
	model_->Draw(world_, view, textureHandle_);
	
	// 弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(view);
	}

}