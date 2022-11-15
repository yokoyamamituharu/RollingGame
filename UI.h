#pragma once
#include "Sprite.h"
class UI
{
public:

	static void Create();

	void Draw();

private:
	void Initialize();

private:
	Sprite* sprite = nullptr;
};