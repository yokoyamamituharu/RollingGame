#include "SpriteManager.h"

SpriteManager::SpriteManager()
{
}

SpriteManager::~SpriteManager()
{
}

SpriteManager* SpriteManager::GetIns()
{
	static SpriteManager instance;
	return &instance;
}

void SpriteManager::Initialize()
{
	//スプライト共通テクスチャ読み込み
	Sprite::LoadTexture(0, L"Resources/texture.png");
	Sprite::LoadTexture(1, L"Resources/torisetu.png");
	Sprite::LoadTexture(2, L"Resources/clear.png");
	Sprite::LoadTexture(3, L"Resources/hp.png");
	Sprite::LoadTexture(4, L"Resources/damageHp.png");
	//背景
	Sprite::LoadTexture(5, L"Resources/title.png");
	Sprite::LoadTexture(6, L"Resources/end.png");
	Sprite::LoadTexture(7, L"Resources/black.png");
	Sprite::LoadTexture(8, L"Resources/kati.png");

	Sprite::LoadTexture(9, L"Resources/num/slash.png");
	Sprite::LoadTexture(10, L"Resources/num/0.png");
	Sprite::LoadTexture(11, L"Resources/num/1.png");
	Sprite::LoadTexture(12, L"Resources/num/2.png");
	Sprite::LoadTexture(13, L"Resources/num/3.png");
	Sprite::LoadTexture(14, L"Resources/num/4.png");
	Sprite::LoadTexture(15, L"Resources/num/5.png");
	Sprite::LoadTexture(16, L"Resources/num/6.png");
	Sprite::LoadTexture(17, L"Resources/num/7.png");
	Sprite::LoadTexture(18, L"Resources/num/8.png");
	Sprite::LoadTexture(19, L"Resources/num/9.png");

	//シーン遷移エフェクト
	Sprite::LoadTexture(21, L"Resources/efect_1.png");
	Sprite::LoadTexture(22, L"Resources/efect_2.png");
	Sprite::LoadTexture(23, L"Resources/efect_3.png");
	Sprite::LoadTexture(24, L"Resources/efect_4.png");
	Sprite::LoadTexture(25, L"Resources/efect_5.png");

	//UI
	Sprite::LoadTexture(26, L"Resources/pose.png");
}
