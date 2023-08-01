﻿#include "Player.h"
#include <cassert>
#include "ImGuiManager.h"

//初期化
void Player::Initialize(Model* model, uint32_t textureHandle, Vector3& position) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	//シングルトンインスタンスを取得する
	input_ = Input::GetInstance();

	// 3Dレティクルのワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	//レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("target.png");

	//スプライト生成
	sprite2DReticle_ = Sprite::Create(textureReticle, {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});

	model3DReticle = model;
}

//デストラクタ
Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		if (bullet) {
			delete bullet;
			bullet = nullptr;
		}
	}

	//2Dレティクルのデストラクタ
	delete sprite2DReticle_;
}

//衝突判定
void Player::OnCollision() {}

//更新
void Player::Update(ViewProjection& viewProjection) {
	//デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// 2-2～
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
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	//旋回
	const float matRotSpeed = 0.02f;
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= matRotSpeed;
	}
	if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += matRotSpeed;
	}

	//キャラクター攻撃処理
	Attack();
	if (bullet_) {
		bullet_->Update();
	}

	//弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	//範囲制限
	const float kMoveLimitX = 34.0f;//34.0f
	const float kMoveLimitY = 18.0f;//18.0f

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//座標移動 (ベクトルの加算)
	worldTransform_.translation_.x += move.x;// ※数学の授業で作った関数を当てはめる
	worldTransform_.translation_.y += move.y;

	//キャラクターの座標を表示
	ImGui::SetNextWindowPos({60, 60});
	ImGui::SetNextWindowContentSize({300, 100});
	ImGui::Begin("Player");
	float sliderValue[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("position", sliderValue, -20.0f, 20.0f);
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};
	ImGui::End();

	//レールカメラと連動
	worldTransform_.UpdateMatrix();

	//自機のワールド座標から3Dレティクルのワールド座標を計算
	//自機から3Dレティクルへの距離
	const float kDistancePlyerTo3DReticle = 10.0f;

	// 自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = {0.0f, 0.0f, 1.0f}; 
	
	//自機のワールド行列の回転を反映
	offset = Transform(offset, worldTransform_.matWorld_);

	//ベクトルの長さを整える
	offset = Multiply2(kDistancePlyerTo3DReticle, Normalize(offset));

	//3Dレティクルの座標を設定
	worldTransform3DReticle_.translation_ = Add(GetWorldPosition(), offset);

	worldTransform3DReticle_.UpdateMatrix();
	worldTransform3DReticle_.TransferMatrix();

	// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	Vector3 positionReticle;
	positionReticle.x = worldTransform3DReticle_.matWorld_.m[3][0];
	positionReticle.y = worldTransform3DReticle_.matWorld_.m[3][1];
	positionReticle.z = worldTransform3DReticle_.matWorld_.m[3][2];

	// ビューポート行列
	Matrix4x4 matViewport =
	    MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);

	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport = viewProjection.matView * viewProjection.matProjection * matViewport;

	// ワールド->スクリーン座標変換 (ここで3Dから2Dになる)
	positionReticle = Transform2(positionReticle, matViewProjectionViewport);

	// スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	
	// 2-15
	//マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	POINT mousePosition;

	//マウス座標(スクリーン座標)を習得する
	GetCursorPos(&mousePosition);

	//クライアントエリアに座標変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	//マウス座標を2Dレティクルのスプライトに代入する
	sprite2DReticle_->SetPosition(
	    Vector2(static_cast<float>(mousePosition.x), static_cast<float>(mousePosition.y)));

	//ビュープロジェクションビューポート合成行列
	//ビュー行列
	Matrix4x4 matV =
	    MakeViewportMatrix(0.0f, 0.0f, WinApp::kWindowWidth, WinApp::kWindowHeight, 0.0f, 1.0f);

	//ビューポート行列
	Matrix4x4 matVPV = viewProjection.matView * viewProjection.matProjection * matV;

	//合成行列の逆行列を計算する
	Matrix4x4 matInverseVPV = Inverse(matVPV);

	//スクリーン座標
	Vector3 posNear = Vector3((float)mousePosition.x, (float)mousePosition.y, 0);
	Vector3 posFar = Vector3((float)mousePosition.x, (float)mousePosition.y, 1);

	//スクリーン座標系からワールド座標系へ
	posNear = Transform2(posNear, matInverseVPV);
	posFar = Transform2(posFar, matInverseVPV);

	//マウスレイの方向
	Vector3 mouseDrection = Subtract(posFar, posNear);
	mouseDrection = Normalize(mouseDrection);
	
	//カメラの照準オブジェクトの距離
	const float kDistanceTestObject = 50.0f;
	worldTransform3DReticle_.translation_ = Add(posNear, Multiply2(kDistanceTestObject, mouseDrection));

	ImGui::Begin("Player");
	ImGui::Text("2DReticle:(%f,%f)", sprite2DReticle_->GetPosition().x, sprite2DReticle_->GetPosition().y);
	ImGui::Text("Near:(%+.2f,%+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,%+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text("3DReticle:(%+.2f,%+.2f,%+.2f)",
		worldTransform3DReticle_.translation_.x,worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
	ImGui::End();

}

//アタック
 void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		// 弾の速度
		const float kBulletSpeed = 1.0f;

		Vector3 velocityReticle;
		// 自機から照準オブジェクトへのベクトル
		velocityReticle.x = worldTransform3DReticle_.matWorld_.m[3][0] - worldTransform_.matWorld_.m[3][0];
		velocityReticle.y = worldTransform3DReticle_.matWorld_.m[3][1] - worldTransform_.matWorld_.m[3][1];
		velocityReticle.z = worldTransform3DReticle_.matWorld_.m[3][2] - worldTransform_.matWorld_.m[3][2];

		velocityReticle = Multiply2(kBulletSpeed, Normalize(velocityReticle));

		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);
	}

	
 }

 //ワールド座標を入れる変数
 Vector3 Player::GetWorldPosition() { 
	 //ワールド座標を入れる変数
	 Vector3 worldPos;
	 
	 //ワールド行列の平行移動成分を取得 (ワールド座標)
	 worldPos.x = worldTransform_.matWorld_.m[3][0];
	 worldPos.y = worldTransform_.matWorld_.m[3][1];
	 worldPos.z = worldTransform_.matWorld_.m[3][2];
	 
	return worldPos; 
 }

 //描画
 void Player::Draw(ViewProjection& viewProjection) { 

	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	model3DReticle->Draw(worldTransform3DReticle_, viewProjection);

	if (bullet_) {
		bullet_->Draw(viewProjection);
	}

	//弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

 }

 //レールカメラとのペアレント
void Player::SetParent(const WorldTransform* parent) { 
	//親子関係を結ぶ
	worldTransform_.parent_ = parent; 

}

void Player::DrawUI() { 
	sprite2DReticle_->Draw();

}
