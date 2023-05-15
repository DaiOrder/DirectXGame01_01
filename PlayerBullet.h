#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class PlayerBullet {
public:
	void Initialize(Model*model,const Vector3& pos);
	void Update();
	void Draw(ViewProjection& view);

private:
	WorldTransform world_;
	Model* model_;
	uint32_t texturehandle_;

};