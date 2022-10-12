#include "Enemy.h"

void Enemy::Initialize(std::string modelName)
{
	if (modelName != "") {
		this->modelName = modelName;
	}
	model = new Model;
	model->CreateFromOBJ(this->modelName);
	object = OBJobject::Create();
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
