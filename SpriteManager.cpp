#include "SpriteManager.h"
#include "Sprite.h"

SpriteManager::SpriteManager()
{
}

SpriteManager::~SpriteManager()
{
}

void SpriteManager::StaticInitialize()
{
	//スプライト共通テクスチャ読み込み
	Sprite::LoadTexture(texture, L"Resources/texture/texture.png");
	Sprite::LoadTexture(torisetu, L"Resources/texture/UI/torisetu.png");
	Sprite::LoadTexture(hp, L"Resources/texture/UI/hp.png");
	Sprite::LoadTexture(damageHp, L"Resources/texture/UI/damageHp.png");
	//背景
	Sprite::LoadTexture(title, L"Resources/texture/title.png");
	Sprite::LoadTexture(make, L"Resources/texture/make.png");
	Sprite::LoadTexture(black, L"Resources/texture/EFFECT/black.png");
	Sprite::LoadTexture(kati, L"Resources/texture/kati.png");

	Sprite::LoadTexture(slash, L"Resources/texture/num/slash.png");
	Sprite::LoadTexture(num0, L"Resources/texture/num/0.png");
	Sprite::LoadTexture(num1, L"Resources/texture/num/1.png");
	Sprite::LoadTexture(num2, L"Resources/texture/num/2.png");
	Sprite::LoadTexture(num3, L"Resources/texture/num/3.png");
	Sprite::LoadTexture(num4, L"Resources/texture/num/4.png");
	Sprite::LoadTexture(num5, L"Resources/texture/num/5.png");
	Sprite::LoadTexture(num6, L"Resources/texture/num/6.png");
	Sprite::LoadTexture(num7, L"Resources/texture/num/7.png");
	Sprite::LoadTexture(num8, L"Resources/texture/num/8.png");
	Sprite::LoadTexture(num9, L"Resources/texture/num/9.png");

	//シーン遷移エフェクト
	Sprite::LoadTexture(effect_1, L"Resources/texture/EFFECT/efect_1.png");
	Sprite::LoadTexture(effect_2, L"Resources/texture/EFFECT/efect_2.png");
	Sprite::LoadTexture(effect_3, L"Resources/texture/EFFECT/efect_3.png");
	Sprite::LoadTexture(effect_4, L"Resources/texture/EFFECT/efect_4.png");
	Sprite::LoadTexture(effect_5, L"Resources/texture/EFFECT/efect_5.png");

	//UI
	Sprite::LoadTexture(pose, L"Resources/texture/UI/pose.png");
	Sprite::LoadTexture(hit, L"Resources/texture/UI/hit.png");
	Sprite::LoadTexture(mousepos, L"Resources/texture/UI/mousepos.png");
	Sprite::LoadTexture(tikai, L"Resources/texture/UI/tikai.png");
	Sprite::LoadTexture(arrowSymbol, L"Resources/texture/UI/arrowSymbol.png");
	Sprite::LoadTexture(sprite_0001, L"Resources/texture/UI/Sprite-0001.png");
	Sprite::LoadTexture(sprite_0002, L"Resources/texture/UI/Sprite-0002.png");
}
