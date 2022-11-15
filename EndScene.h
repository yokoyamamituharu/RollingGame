#pragma once
#include "Sprite.h"
#include "DirectXCommon.h"

class EndScene
{
public:
	void Initialize(DirectXCommon* dxCommon);

	void Update(int& sceneNo);

	void Draw();

private:
	Sprite* sprite = nullptr;
	DirectXCommon* dxCommon = nullptr;
};

