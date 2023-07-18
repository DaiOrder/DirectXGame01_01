#include "Enemy.h"
#include <cassert>
#include <ImGuiManager.h>
#include "EnemyBullet.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Player.h"

// 初期化
void Enemy::Initialize(Model* model, const Vector3& pos) {
	assert(model);
	model_ = model;

	textureHandle_ = TextureManager::Load("sample.png");

	worldTransform_.Initialize();
	worldTransform_.translation_ = pos;

	PhaseInitialize();

}

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		if (bullet->IsDead()) {
			delete bullet;
		}
	}
}

Vector3 Enemy::GetWorldPosition() { 
	// ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得 (ワールド座標)
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Enemy::OnCollision() {}

// アプデ
void Enemy::Update() {
	worldTransform_.UpdateMatrix();

	const float kEnemySpeed = -0.2f;
	velocity_ = {0, 0, kEnemySpeed};

	switch (phase_) { 
	case Phase::Approach:
	default:
		// 移動 (ベクトルを加算)
		worldTransform_.translation_.x += velocity_.x;
		worldTransform_.translation_.y += velocity_.y;
		worldTransform_.translation_.z += velocity_.z;

		if (worldTransform_.translation_.z < -10.0f) {
			phase_ = Phase::Leave;
		}
		break;

	case Phase::Leave:
		velocity_.x = 0.2f;
		velocity_.y = 0.1f;

		// 移動 (ベクトルを加算)
		worldTransform_.translation_.x += velocity_.x;
		worldTransform_.translation_.y += velocity_.y;
		worldTransform_.translation_.z += velocity_.z;
		break;
	}

	ImGui::Begin("Window");
	ImGui::DragFloat3("world_.translation_.z", &worldTransform_.translation_.x, 0.01f);
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

	assert(player_);
	const float kBulletSpeed = 0.8f;

	//ワールド座標を取得
	Vector3 playerWorldPos = player_->GetWorldPosition();	//プレイヤー
	Vector3 enemyWorldPos = GetWorldPosition();				//エネミー

	//差分ベクトルの抽出
	Vector3 deltaVector_;
	deltaVector_.x = playerWorldPos.x - enemyWorldPos.x;
	deltaVector_.y = playerWorldPos.y - enemyWorldPos.y;
	deltaVector_.z = playerWorldPos.z - enemyWorldPos.z;

	//正規化
	float length = sqrt(
	deltaVector_.x * deltaVector_.x + deltaVector_.y * deltaVector_.y + deltaVector_.z * deltaVector_.z);

	deltaVector_.x /= length;
	deltaVector_.y /= length;
	deltaVector_.z /= length;

	//移動ベクトルの座標を求める
	deltaVector_.x *= kBulletSpeed;
	deltaVector_.y *= kBulletSpeed;
	deltaVector_.z *= kBulletSpeed;

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, deltaVector_);

	// 弾を登録する
	bullets_.push_back(newBullet);

}

// 描画
void Enemy::Draw(ViewProjection& view) { 
	model_->Draw(worldTransform_, view, textureHandle_);
	
	// 弾描画
	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(view);
	}

}