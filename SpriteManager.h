#pragma once
#include "Sprite.h"

class SpriteManager
{
public:
	SpriteManager();
	~SpriteManager();

	static SpriteManager* GetIns();

	static void Initialize();
};

