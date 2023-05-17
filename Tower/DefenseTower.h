#pragma once
#include "ObjectObj.h"
#include "Bullet.h"
#include "Player.h"
#include "EnemyZako.h"
class DefenseTower
{
	enum class TowerType {
		NONE_TOWER,
		HEAVEY_TOWER,
		LIGHT_TOWER,
		MIDDLE_TOWER
	};

	enum class State {
		none,
		idle,
		attack,
	};

public:
	static DefenseTower* Create();

public:
	DefenseTower();
	~DefenseTower();

	void Update(std::list<std::shared_ptr<BaseEnemy>>& enemies);
	void Draw();
	ObjectObj* GetObjectObj() { return object; }

private:
	bool Initialize();

private:
	//TowerType type = TowerType::NONE_TOWER;
	State state = State::none;

	ObjectObj* object = nullptr;
	bool attackFlag = false;
	const int maxHp = 10;
	int hp = maxHp;
	std::list<std::unique_ptr<Bullet>>bullets;

	const int maxInterval = 120;
	int interval = maxInterval;

	std::weak_ptr<BaseEnemy> targetEnemy;	
};

