#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

#include "Player.h"
#include <DebugCamera.h>
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"
#include "sstream"


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//敵弾を追加する
	void AddEnemyBullet(EnemyBullet* enemyBullet);
	
	//敵の弾のリストお引越し
	std::list<EnemyBullet*> bullets_;

	// 弾リストを取得
	const std::list<EnemyBullet*> GetBullets() const { return bullets_; }

	std::list<Enemy*> enemy_;

/// <summary>
/// 衝突判定と応答
/// </summary>
	void CheckAllCollisions();

	//敵発生データの読み込み
	void LoadEnemyPopDate();

	//敵発生コマンドの更新
	void UpdateEnemyPopCommands();

	// 敵を発生させる
	void AddEnemy(Vector3 xyz);

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	//2-1～
	uint32_t textureHandle_ = 0;
	ViewProjection viewProjection_;
	Model* model_ = nullptr;
	Player* player_ = nullptr;
	//Enemy* enemy_ = nullptr;

	// 2-2～
	bool isDebugCameraActve_ = false;
	DebugCamera* debugCamera_ = nullptr;

	// 2-11
	//3Dモデルデータ
	Model* modelSkydome_ = nullptr;
	//3Dモデルデータの生成
	Skydome* skydome_ = nullptr;

	// 2-12
	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	std::stringstream enemyPopCommands;

	//待機中フラグ・待機タイマー
	bool waitFlag_ = false;
	int waitTimer_ = 0;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

};
