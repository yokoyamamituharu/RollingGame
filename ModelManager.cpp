#include "ModelManager.h"

Model* ModelManager::bulletModel = nullptr;

void ModelManager::Initialize()
{
	//’e—pƒ‚ƒfƒ‹‚ð“Ç‚Ýž‚Ý
	bulletModel = new Model();
	bulletModel->CreateFromOBJ("bullet");
}

ModelManager* ModelManager::GetIns()
{
	static ModelManager instance;
	return &instance;
}
