﻿#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Input.h"
#include "EnemyBullet.h"
#include "Effect.h"

#include <list>

// 自クラスの前方宣言
class Player;

//ゲームシーンクラスの前方宣言
class GameScene;

class Effect;

class Enemy {
public:

	//初期化
	void Initialize(Model* model, const Vector3& pos);

	//アプデ
	void Update();

	//描画
	void Draw(ViewProjection& view);
	
	// 弾発射
	void Fire();

	// フェーズの初期化
	void PhaseInitialize();

	//デストラクタ
	~Enemy();

	// 発射間隔
	static const int kFireInterval = 50;

	void SetPlayer(Player* player) { player_ = player; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	//行動フェーズ
	enum class Phase {
		Approach, //接近する
		Leave,	  //離脱する
	};

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();
	
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool GetIsDead() { return isDead_;};

private:
	//ワールドトランスフォーム
	WorldTransform worldTransform_;

	//モデル
	Model* model_;

	//リソース
	uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 velocity_;

	//フェーズ
	Phase phase_ = Phase::Approach;
	
	EnemyBullet* bullet_ = nullptr;
	std::list<EnemyBullet*> bullets_;

	//タイマー
	int32_t Timer = 0;

	// キーボード入力
	Input* input_ = nullptr;
	
	// 自キャラ
	Player* player_ = nullptr;

	//ゲームシーン
	GameScene* gameScene_ = nullptr;
	
	Vector3 deltaVector_;

	Effect* effect_ = nullptr;

	//デスフラグ
	bool isDead_ = false;

	//デスタイマー
	int deadTimer_ = 300;

	//エネミーHP
	int enemyHp_;

};
