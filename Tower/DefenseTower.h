#pragma once
#include "OBJObject.h"
#include "Bullet.h"
#include "Player.h"
#include "EnemyZako.h"
class DefenseTower
{
	enum TowerType {
		NONE_TOWER,
		HEAVEY_TOWER,
		LIGHT_TOWER,
		MIDDLE_TOWER
	};

public:
	static DefenseTower* Create();

public:
	void Update(std::list<std::shared_ptr<EnemyZako>>& enemies);
	void Draw();
	OBJobject* GetOBJObject() { return object; }

private:
	bool Initialize();

private:
	int type = NONE_TOWER;
	Model* model = nullptr;
	OBJobject* object = nullptr;
	bool attackFlag = false;
	const int maxHp = 10;
	int hp = maxHp;
	std::list<std::unique_ptr<Bullet>>bullets;

	const int maxInterval = 10;
	int interval = maxInterval;

	std::weak_ptr<EnemyZako> targetEnemy;
};

