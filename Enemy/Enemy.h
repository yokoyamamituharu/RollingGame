#pragma once
#include "ObjectObj.h"
class Enemy
{
public:
	Enemy();
	~Enemy();

	void Initialize(std::string modelName = "");

	void Update();

	void Draw();
private:
	Model* model = nullptr;
	ObjectObj* object = nullptr;
	std::string modelName = "enemy";

	const int maxHp = 5;
	int hp = maxHp;
};

