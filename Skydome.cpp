#include "Skydome.h"
#include <cassert>

//初期化
void Skydome::Initialize(Model* model) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
}

//更新
void Skydome::Update() {}

//描画
void Skydome::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection);

}
