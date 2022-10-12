#include "ModelManager.h"

Model* ModelManager::bulletModel = nullptr;

void ModelManager::Initialize()
{
	//’e—pƒ‚ƒfƒ‹‚ğ“Ç‚İ‚İ
	bulletModel = Model::Create("bullet");
}

ModelManager* ModelManager::GetIns()
{
	static ModelManager instance;
	return &instance;
}
