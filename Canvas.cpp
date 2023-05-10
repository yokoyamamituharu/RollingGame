#include "Canvas.h"
#include "safe_delete.h"

void Canvas::Initialize()
{
	float hpWidth = 100;
	float hpHegiht = 650;

	for (int i = 0; i < sizeof(playerHp) / sizeof(playerHp[0]); i++) {
		playerHp[i] = Sprite::Create(SpriteManager::hp, { hpWidth + float(i * 35),hpHegiht });
		damagePlayerHp[i] = Sprite::Create(SpriteManager::damageHp, { hpWidth + float(i * 35),hpHegiht });
	}

	int intervalWidth = 30;
	float fintervalWidth = float(intervalWidth);
	float intervalWidth2 = 25;

	float height = 600;
	float width = 500;

	for (int i = 0; i < 10; i++) {
		enemyNum3[i] = Sprite::Create(SpriteManager::num0 + i, { width + fintervalWidth * 3 + float(intervalWidth2 * 0),height });
		enemyNum2[i] = Sprite::Create(SpriteManager::num0 + i, { width + fintervalWidth * 3 + float(intervalWidth2 * 1),height });
		enemyNum1[i] = Sprite::Create(SpriteManager::num0 + i, { width + fintervalWidth * 3 + float(intervalWidth2 * 2),height });
		enemyNum3[i]->SetScale({ 1.6,1.6 });
		enemyNum2[i]->SetScale({ 1.6,1.6 });
		enemyNum1[i]->SetScale({ 1.6,1.6 });

		enemyNum3[i]->SetColor({ 0.9,0.9,0.9 });
		enemyNum2[i]->SetColor({ 0.9,0.9,0.9 });
		enemyNum1[i]->SetColor({ 0.9,0.9,0.9 });

		breakEnemyNum3[i] = Sprite::Create(SpriteManager::num0 + i, { width + float(intervalWidth * 0),height });
		breakEnemyNum2[i] = Sprite::Create(SpriteManager::num0 + i, { width + float(intervalWidth * 1),height });
		breakEnemyNum1[i] = Sprite::Create(SpriteManager::num0 + i, { width + float(intervalWidth * 2),height });
		breakEnemyNum3[i]->SetScale({ 2,2 });
		breakEnemyNum2[i]->SetScale({ 2,2 });
		breakEnemyNum1[i]->SetScale({ 2,2 });
	}

	slash = Sprite::Create(SpriteManager::slash, { width + 30 * 3,height });
	slash->SetScale({ 1.6,1.6 });
	slash->SetColor({ 0.9,0.9,0.9 });
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

	int num1 = maxEnemyNum % 10;
	int num2 = maxEnemyNum / 10;
	int num3 = maxEnemyNum / 100;

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

Canvas::Canvas()
{
}

Canvas::~Canvas()
{
	for (int i = 0; i < limitHp; i++) {
		safe_delete(playerHp[i]);
		safe_delete(damagePlayerHp[i]);
	}

	for (int i = 0; i < 10; i++) {
		safe_delete(enemyNum1[i]);
		safe_delete(enemyNum2[i]);
		safe_delete(enemyNum3[i]);
		safe_delete(breakEnemyNum1[i]);
		safe_delete(breakEnemyNum2[i]);
		safe_delete(breakEnemyNum3[i]);
	}
	safe_delete(slash);
}
