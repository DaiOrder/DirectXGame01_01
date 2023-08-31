#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include<math.h>
#include "AxisIndicator.h"

#include "Enemy.h"
#include "Player.h"
#include "fstream"

GameScene::GameScene() { 

}

GameScene::~GameScene() {
	delete player_;

	delete model_;

	// 2-2～
	delete debugCamera_;
	
	// 02-11～
	delete modelSkydome_;

	delete modelEnemy_;

	for (EnemyBullet* bullet : bullets_) {
			delete bullet;
	}

	for (Enemy* enemy : enemy_) {
		delete enemy;
	}

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//自機リソース
	//textureHandle_ = TextureManager::Load("tex.png");
	modelPlyer_ = Model::CreateFromOBJ("Neko2");
	//Model::CreateFromOBJ
	
	// ビュープロジェクションの初期化
	viewProjection_.farZ = 150;
	viewProjection_.Initialize();

	model_ = Model::Create();

	//自キャラの生成
	player_ = new Player();

	// 自機をカメラからずらす
	Vector3 playerPosition(0.0f, 0.0f, 30.0f);

	//自キャラの初期化
	player_->Initialize(modelPlyer_, playerPosition);

	// 2-2～
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// 3Dモデルの生成 : スカイドーム
	modelSkydome_ = Model::CreateFromOBJ("skydome");

	//スカイドームの生成
	skydome_ = new Skydome();
	
	//スカイドームの初期化
	skydome_->Initialize(modelSkydome_);

	//レールカメラの生成
	railCamera_ = new RailCamera();

	//レールカメラの初期化
	railCamera_->Initialize({0.0f, 0.0f,-55.0f}, {0.0f, 0.0f, 0.0f});

	//自キャラとレールカメラの親子関係を結ぶ
	player_->SetParent(&railCamera_->GetWorldTransform());

	//エフェクトの生成
	/*effect_ = new Effect();
	modelEffect_ = Model::CreateFromOBJ("effect");
	effect_->Initialize(modelEffect_, {0, 0, 0}, {0, 0, 0});*/

	modelEnemy_ = Model::CreateFromOBJ("mouse", true);

	//CSVデータを読み込む
	LoadEnemyPopDate();

	AddEnemy({0.0f, 5.0f, 50.0f});

	//レティクルのテクスチャ
	TextureManager::Load("target.png");
	
	enemyDead_ = 0;
	playerDead_ = 0;

	tex_[0] = TextureManager::Load("start.png");
	tex_[1] = TextureManager::Load("ex.png");
	tex_[2] = TextureManager::Load("finish.png");
	tex_[3] = TextureManager::Load("dead.png");

	tex1 = Sprite::Create(tex_[0], {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});
	tex2 = Sprite::Create(tex_[1], {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});
	tex3 = Sprite::Create(tex_[2], {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});
	tex4 = Sprite::Create(tex_[3], {640, 360}, {1, 1, 1, 1}, {0.5f, 0.5f});

	Hp_[0] = TextureManager::Load("Hp.png");
	Hp_[1] = TextureManager::Load("Hp.png");
	Hp_[2] = TextureManager::Load("Hp.png");
	Hp_[3] = TextureManager::Load("Hp.png");
	Hp_[4] = TextureManager::Load("Hp.png");

	Hp1 = Sprite::Create(Hp_[0], {740, 660}, {1, 1, 1, 1}, {0.5f, 0.5f});
	Hp2 = Sprite::Create(Hp_[1], {690, 660}, {1, 1, 1, 1}, {0.5f, 0.5f});
	Hp3 = Sprite::Create(Hp_[2], {640, 660}, {1, 1, 1, 1}, {0.5f, 0.5f});
	Hp4 = Sprite::Create(Hp_[3], {590, 660}, {1, 1, 1, 1}, {0.5f, 0.5f});
	Hp5 = Sprite::Create(Hp_[4], {540, 660}, {1, 1, 1, 1}, {0.5f, 0.5f});

	timePhase_ = 0;
}

void GameScene::Update() {

	if (phase_ == Phase::title) {

		if (input_->TriggerKey(DIK_SPACE)) {
			phase_ = Phase::explanation;
		}

	}

	if (phase_ == Phase::explanation) {
		timePhase_ += 1;
		if (input_->TriggerKey(DIK_SPACE) && timePhase_>=10) {
			phase_ = Phase::game;
		}
	
	}

	if (phase_ == Phase::game) {
		// 自キャラの更新
		player_->Update(viewProjection_);

		// 敵ポップ
		UpdateEnemyPopCommands();

		// 敵の更新
		for (Enemy* enemy : enemy_) {
			enemy->Update();
		}

		// 敵の弾更新
		for (EnemyBullet* bullet : bullets_) {
			bullet->Update();
		}

		// 当たり判定
		CheckAllCollisions();

		// スカイドーム
		skydome_->Update();

		// エフェクト
		// effect_->Update();

		// レールカメラ
		railCamera_->Update();

		// デバックカメラ
		debugCamera_->Update();

#ifdef _DEBUG
		if (input_->TriggerKey(DIK_BACKSPACE)) {
			isDebugCameraActve_ = true;
		}

#endif

		if (isDebugCameraActve_) {
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;

			viewProjection_.TransferMatrix();
		} else {

			viewProjection_.UpdateMatrix();
		}

		// レールカメラの描画を反映
		viewProjection_.matView = railCamera_->GetRailView().matView;
		viewProjection_.matProjection = railCamera_->GetRailView().matProjection;

		viewProjection_.TransferMatrix();

		// デスフラグの立った弾を削除
		bullets_.remove_if([](EnemyBullet* bullet) {
			if (bullet->IsDead()) {
				delete bullet;
				return true; // 要素を削除するために true を返す
			}
			return false; // 要素を削除しないために false を返す
		});

		// デスフラグの立った敵を削除
		enemy_.remove_if([](Enemy* enemy_) {
			if (enemy_->GetIsDead()) {
				delete enemy_;
				return true;
			}

			return false;
		});

		if (enemyDead_ == 20) {
			phase_ = Phase::finish;
		}

		if (playerDead_ == 5) {
			phase_ = Phase::dead;
		}
	
	}

	if (phase_ == Phase::finish) {
		if (input_->TriggerKey(DIK_SPACE)) {
			phase_ = Phase::reset;
		}

	}

	if (phase_ == Phase::dead) {
		if (input_->TriggerKey(DIK_SPACE)) {
			phase_ = Phase::reset;
		}
	}

	if (phase_ == Phase::reset && phase_ != Phase::game) {
		ResetInitialize();
	}
	
}

void GameScene::ResetInitialize() {
	phase_ = Phase::title;

	ResetEnemies();
	Initialize();
	
}


void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	if (phase_ == Phase::title) {
		tex1->Draw();
	}
	if (phase_ == Phase::explanation) {
		tex2->Draw();
	}
	if (phase_ == Phase::finish) {
		tex3->Draw();
	}
	if (phase_ == Phase::dead) {
		tex4->Draw();
	}

	

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	if (phase_ == Phase::game) {
		// スカイドーム
		skydome_->Draw(viewProjection_);

		// 弾描画
		for (EnemyBullet* bullet : bullets_) {
			bullet->Draw(viewProjection_);
		}

		// 敵の描画
		for (Enemy* enemy : enemy_) {
			enemy->Draw(viewProjection_);
		}

		// 自キャラの描画
		player_->Draw(viewProjection_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	if (phase_ == Phase::game) {
		// 2Dのレティクルの描画
		player_->DrawUI();
	}

	if (phase_ == Phase::game) {

		if (playerDead_ <= 0) {
			Hp1->Draw();
		}
		if (playerDead_ <= 1) {
			Hp2->Draw();
		}
		if (playerDead_ <= 2) {
			Hp3->Draw();
		}
		if (playerDead_ <= 3) {
			Hp4->Draw();
		}
		if (playerDead_ <= 4) {
			Hp5->Draw();
		}
	}

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	//リストに登録する
	bullets_.push_back(enemyBullet);

}

void GameScene::CheckAllCollisions() { 
	//判定対象AとBの座標
	Vector3 posA, posB;

	//自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();


#pragma region 自キャラと敵弾の当たり判定
	//自キャラの座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : bullets_) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		//座標 A・B の距離を求める
		Vector3 dis;
		dis.x = posA.x - posB.x;
		dis.y = posA.y - posB.y;
		dis.z = posA.z - posB.z;

		float distance = sqrt((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z));

		if (distance <= 3) {
			//自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();

			//敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
			
			playerDead_ += 1;
		}

	}

#pragma endregion

#pragma region 自弾と敵キャラの当たり判定

	// 敵キャラの座標
	for (Enemy* enemy : enemy_) {
		posA = enemy->GetWorldPosition();

	// 敵キャラと自弾全ての当たり判定
		for (PlayerBullet* bullet : playerBullets) {
			// 自弾の座標
			posB = bullet->GetWorldPosition();

			// 座標 A・B の距離を求める
			Vector3 dis;
			dis.x = posA.x - posB.x;
			dis.y = posA.y - posB.y;
			dis.z = posA.z - posB.z;

			float distance = sqrt((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z));

			if (distance <= 3) {
				// 敵キャラの衝突時コールバックを呼び出す
				enemy->OnCollision();

				// 自弾の衝突時コールバックを呼び出す
				bullet->OnCollision();
				
				enemyDead_ += 1;
				//effect_->SetWorldPosition(enemyPos_);
			}
		}
	}
	

#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	// 自弾と敵弾全ての当たり判定
	 for (PlayerBullet* bulletPlayer : playerBullets) {
		posA = bulletPlayer->GetWorldPosition();

		for (EnemyBullet* bulletEnemy : bullets_) {
			// 自弾の座標
			posB = bulletEnemy->GetWorldPosition();

			// 座標 A・B の距離を求める
			Vector3 dis;
			dis.x = posA.x - posB.x;
			dis.y = posA.y - posB.y;
			dis.z = posA.z - posB.z;

			float distance = sqrt((dis.x * dis.x) + (dis.y * dis.y) + (dis.z * dis.z));

			if (distance <= 3) {
				// 敵キャラの衝突時コールバックを呼び出す
				bulletPlayer->OnCollision();

				// 自弾の衝突時コールバックを呼び出す
				bulletEnemy->OnCollision();
			}
		}
	}
#pragma endregion

}

void GameScene::LoadEnemyPopDate() {
	//ファイルを開く
	std::ifstream file;

	file.open("Resources/enemyPOP.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();


}

void GameScene::UpdateEnemyPopCommands() {
	//待機処理
	if (waitFlag_) {
		waitTimer_--;
		if (waitTimer_ <= 0) {
			//待機完了
			waitFlag_ = false;
		}
		return;
	}

	//１行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		//１行分の文字列をストリームに変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0) {
			// x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			
			// y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			// 敵を発生させる
			AddEnemy(Vector3(x, y, z));
		}
		// WATEコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			// 待ち時間
			int32_t waitTime = atoi(word.c_str());

			// 待機開始
			waitFlag_ = true;
			waitTimer_ = waitTime;

			// コマンドループを抜ける
			break;

		}

	}
	

}

void GameScene::AddEnemy(Vector3 xyz) { 
	Enemy* obj = new Enemy();

	obj->Initialize(modelEnemy_, xyz);

	obj->SetPlayer(player_);

	obj->SetGameScene(this);

	enemy_.push_back(obj);

}

void GameScene::ResetEnemies() {
	// すべての敵を削除
	for (Enemy* enemy : enemy_) {
		delete enemy;
	}
	enemy_.clear();

	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
	bullets_.clear();
	
}

