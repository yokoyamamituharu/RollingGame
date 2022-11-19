#pragma once
#include "GameScene.h"
#include "BatlleScene.h"
#include "TitleScene.h"
#include "EndScene.h"
#include "ModelManager.h"

class SceneManager
{
public:
	enum SCENE
	{
		SCENE_NONE,
		SCENE_TITLE,
		SCENE_GAME,
		SCENE_BATTLE,
		SCENE_END,
		SCENE_KATI
	};

public:
	void Initialize(DirectXCommon* dxCommon, Input* input, InputMouse* mouse, Camera* camera);
	void Update(DirectXCommon* dxCommon, Input* input, InputMouse* mouse, Camera* camera);
	void Draw();
	SceneManager();
	~SceneManager();
public:
	GameScene* gameScene = nullptr;
	BatlleScene* batlleScene = nullptr;
	TitleScene* titleScene = nullptr;
	EndScene* endScene = nullptr;

	int sceneNo = SCENE_NONE;

	bool initFlag = 0;
};

