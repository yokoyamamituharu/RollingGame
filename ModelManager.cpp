#include "ModelManager.h"

Model* ModelManager::bulletModel = nullptr;

void ModelManager::Initialize()
{
	//�e�p���f����ǂݍ���
	bulletModel = new Model();
	bulletModel->CreateFromOBJ("bullet");
}

ModelManager* ModelManager::GetIns()
{
	static ModelManager instance;
	return &instance;
}
