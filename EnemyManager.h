#pragma once
#include "Enemy.h"
class EnemyManager
{
public:
	void Initialize();

	void Update();

	void Draw();

private:
	std::list<std::unique_ptr<Enemy>>enemies;
};