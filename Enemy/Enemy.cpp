#include "Enemy.h"
#include "ModelManager.h"
#include "safe_delete.h"

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
	safe_delete(object);
}

void Enemy::Initialize(std::string modelName)
{
	if (modelName != "") {
		this->modelName = modelName;
	}
	model = ModelManager::GetModel(this->modelName);
	object = ObjectObj::Create();
	object->SetModel(model);
}

void Enemy::Update()
{
	object->Update();
}

void Enemy::Draw()
{
	object->Draw();
}
