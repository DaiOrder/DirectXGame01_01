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
#include "Effect.h"

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

	void ResetEnemies();

	void ResetInitialize();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	//2-1～
	uint32_t textureHandle_ = 0;

	uint32_t tex_[4] = {0};
	Sprite* tex1 = nullptr;
	Sprite* tex2 = nullptr;
	Sprite* tex3 = nullptr;
	Sprite* tex4 = nullptr;

	
	uint32_t Hp_[5] = {0};
	Sprite* Hp1 = nullptr;
	Sprite* Hp2 = nullptr;
	Sprite* Hp3 = nullptr;
	Sprite* Hp4 = nullptr;
	Sprite* Hp5 = nullptr;

	ViewProjection viewProjection_;
	Model* model_ = nullptr;
	Player* player_ = nullptr;
	Effect* effect_ = nullptr;
	Enemy* enemy__ = nullptr;

	// 2-2～
	bool isDebugCameraActve_ = false;
	DebugCamera* debugCamera_ = nullptr;

	// 2-11
	//3Dモデルデータ
	Model* modelSkydome_ = nullptr;
	Model* modelPlyer_ = nullptr;
	Model* modelEffect_ = nullptr;
	Model* modelEnemy_ = nullptr;

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

	enum class Phase { 
		title,//タイトル
		explanation,//操作説明
		game,//プレイ
		finish,//終了
		dead,//死亡
		reset//リセット
	};

	Phase phase_ = Phase::title;

	public:
	int enemyDead_ = 0;
	int playerDead_ = 0;

	int timePhase_;



};
