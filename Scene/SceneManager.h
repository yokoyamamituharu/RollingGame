#pragma once
#include "GameScene.h"
#include "BatlleScene.h"



class SceneManager
{
public:
	enum SCENE
	{
		NONESCENE,
		GAMESCENE,
		BATTLESCENE
	};

public:
	void Initialize(DirectXCommon* dxCommon, Input* input, InputMouse* mouse, Camera* camera);
	void Update(DirectXCommon* dxCommon, Input* input, InputMouse* mouse, Camera* camera);
	void Draw();
private:
	GameScene* gameScene = nullptr;
	BatlleScene* batlleScene = nullptr;

	int sceneNo = NONESCENE;
};

