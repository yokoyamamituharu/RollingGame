#pragma once
#include "GameScene.h"
#include "BatlleScene.h"



class SceneManager
{
public:
	enum SCENE
	{
		SCENE_NONE,
		SCENE_GAME,
		SCENE_BATTLE
	};

public:
	void Initialize(DirectXCommon* dxCommon, Input* input, InputMouse* mouse, Camera* camera);
	void Update(DirectXCommon* dxCommon, Input* input, InputMouse* mouse, Camera* camera);
	void Draw();
private:
	GameScene* gameScene = nullptr;
	BatlleScene* batlleScene = nullptr;

	int sceneNo = SCENE_NONE;
};

