#pragma once
#include "GameScene.h"
#include "BatlleScene.h"
#include "TitleScene.h"
#include "EndScene.h"
#include "ModelManager.h"
#include "PostEffect.h"

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
	SceneManager();
	~SceneManager();

	void Initialize(DirectXCommon* dxCommon);
	void Update();
	void Draw();

	void GameSceneReset();
public:
	GameScene* gameScene = nullptr;
	BatlleScene* batlleScene = nullptr;
	TitleScene* titleScene = nullptr;
	EndScene* endScene = nullptr;

	int sceneNo = SCENE_NONE;

	bool initFlag = 0;

	DirectXCommon* dxCommon = nullptr;
	Camera* camera = nullptr;
	PostEffect* post;

	static bool blackStartFlag;//�u���b�N�A�E�g�J�n
	bool blackFlag = false;		//��ʂ��Ó]���Ă��邩
	bool loadEndFlag = false;		//���[�h�������I�������
	int waitTimer = 10;
	float blackTime = 1.0f;
	float blackTime2 = 0.0f;
};

