#include "ModelManager.h"

Model* ModelManager::bulletModel = nullptr;

void ModelManager::Initialize()
{
	//弾用モデルを読み込み
	bulletModel = new Model();
	bulletModel->CreateFromOBJ("bullet");
}

ModelManager* ModelManager::GetIns()
{
	static ModelManager instance;
	return &instance;
}
