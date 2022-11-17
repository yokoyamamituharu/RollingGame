#pragma once
#include "Sprite.h"
#include "DirectXCommon.h"

class EndScene
{
public:
	void Initialize(DirectXCommon* dxCommon);

	void Update(int& sceneNo);

	void Draw(int& sceneNo);

private:
	Sprite* sprite = nullptr;
	Sprite* katisprite = nullptr;
	DirectXCommon* dxCommon = nullptr;
};

