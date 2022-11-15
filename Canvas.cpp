#include "Canvas.h"

void Canvas::Initialize()
{
	for (int i = 0; i < sizeof(playerHp) / sizeof(playerHp[0]); i++) {
		playerHp[i] = Sprite::Create(3, { float(i * 35),500 });
		damagePlayerHp[i] = Sprite::Create(4, { float(i * 35),500 });
	}

	int intervalWidth = 15;
	float fintervalWidth = float(intervalWidth);
	float height = 500;

	float width = 200;

	for (int i = 0; i < 10; i++) {
		enemyNum3[i] = Sprite::Create(10 + i, { width + fintervalWidth * 3 + float(i * intervalWidth),height });
		enemyNum2[i] = Sprite::Create(10 + i, { width + fintervalWidth * 3 + float(i * intervalWidth),height });
		enemyNum1[i] = Sprite::Create(10 + i, { width + fintervalWidth * 3 + float(i * intervalWidth),height });

		breakEnemyNum3[i] = Sprite::Create(10 + i, { width + float(i * intervalWidth),height });
		breakEnemyNum2[i] = Sprite::Create(10 + i, { width + float(i * intervalWidth),height });
		breakEnemyNum1[i] = Sprite::Create(10 + i, { width + float(i * intervalWidth),height });
	}

	slash = Sprite::Create(9, { width + fintervalWidth * 3,height });
}

void Canvas::Update()
{
}

void Canvas::Draw()
{
	int num = nowHp;
	for (int i = 0; i < maxHp; i++) {
		if (num > 0) {
			playerHp[i]->Draw();
			num--;
		}
		else {
			damagePlayerHp[i]->Draw();
		}
	}

	int num1 = enemyNum % 10;
	int num2 = enemyNum / 10;
	int num3 = enemyNum / 100;

	enemyNum1[num1]->Draw();
	enemyNum2[num2]->Draw();
	//enemyNum1[num3]->Draw();

	int breaknum1 = breakEnemyNum % 10;
	int breaknum2 = breakEnemyNum / 10;
	int breaknum3 = breakEnemyNum / 100;
	breakEnemyNum1[breaknum1]->Draw();
	breakEnemyNum2[breaknum2]->Draw();
	//breakEnemyNum3[breaknum3]->Draw();

	slash->Draw();
}
