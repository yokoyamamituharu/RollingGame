#include "ModelManager.h"

Model* ModelManager::bulletModel = nullptr;

void ModelManager::Initialize()
{
	//�e�p���f����ǂݍ���
	bulletModel = Model::Create("bullet");
}

ModelManager* ModelManager::GetIns()
{
	static ModelManager instance;
	return &instance;
}
