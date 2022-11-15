#pragma once
#include "Sprite.h"
class Canvas
{
public:
	void Initialize();
	void Update();
	void Draw();

	void SetHp(int playerMaxHp, int playerHp) {
		maxHp = playerMaxHp;
		nowHp = playerHp;
	}

private:
	// const int maxHp = 10;
	static const int limitHp = 20;
	int maxHp = 0;
	int nowHp = 0;
	Sprite* playerHp[limitHp] = { nullptr };
	Sprite* damagePlayerHp[limitHp] = { nullptr };

	Sprite* enemyNum1[10] = { nullptr };
	Sprite* enemyNum2[10] = { nullptr };
	Sprite* enemyNum3[10] = { nullptr };

	Sprite* breakEnemyNum1[10] = { nullptr };
	Sprite* breakEnemyNum2[10] = { nullptr };
	Sprite* breakEnemyNum3[10] = { nullptr };

	Sprite* slash = nullptr;

	int enemyNum = 12;
	int breakEnemyNum = 12;

	//int array_tin[maxHp];
};

