#include "Player.h"
#include <assert.h>

void Player::Initialize(Model* model, uint32_t textureHandle) {
	assert(model); 
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update() {
	worldTransform_.TransferMatrix();

	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	//キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	//移動ベクトルの変更(左右)
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	//移動ベクトルの変更(上下)
	if (input_->PushKey(DIK_UP)) {
		move.y -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y += kCharacterSpeed;
	}

	//座標移動 (ベクトルの加算)
	//worldTransform_.translation_ += move.x;// ※数学の授業で作った関数を当てはめる

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation);
}

void Player::Draw(ViewProjection& viewProjection) { 

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
