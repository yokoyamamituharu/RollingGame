#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "InputMouse.h"
#include "PostEffect.h"
#include "Camera.h"
#include "Sprite.h"
#include "ObjectOBJ.h"
#include "ObjectFBX.h"
#include "FbxLoader.h"
#include "Player.h"
#include "EnemyZako.h"
#include "DefenseTower.h"
#include "Enemy.h"
#include "BatlleScene.h"
class BattleScene;
class SceneManeger;
#include <vector>
#include "PostEffect.h"
#include "../Canvas.h"

class GameScene
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//コンストラクタ
	GameScene();
	//デストラクタ
	~GameScene();
	//初期化
	void Initialize(DirectXCommon* dxCommon, Input* input, InputMouse* mouse, Camera* camera);
	//更新
	void Update(int& sceneNo, BatlleScene* batlleScene);
	//描画
	void Draw();

	void SetEnemy(std::shared_ptr<EnemyZako>& enemy) {
		enemiesG.push_back(std::move(enemy));
	}

	//ポストエフェクトの準備
	void PostReserve();

	//ゲームシーンのポストエフェクトを描画
	void PostDraw();

public:
	Player* GetPlayer() { return player; }

private: // メンバ変数
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	InputMouse* mouse = nullptr;
	Camera* camera = nullptr;

	// ゲームシーン用  
	//スプライト
	Sprite* spriteBG = nullptr;
	Sprite* clearsprite = nullptr;
	//3Dオブジェクト
	FbxModel* fbxmodel = nullptr;
	ObjectFBX* objectFBX = nullptr;

	//プレイヤー
	Player* player = nullptr;
	//地面
	ObjectObj* ground = nullptr;
	//敵
	//タワー
	DefenseTower* defenseTower = nullptr;
	Bullet* bullet = nullptr;
	//EnemyZako* enemy1 = nullptr;
	//お城
	ObjectObj* castle = nullptr;

	//巣穴
	ObjectObj* suana = nullptr;
	ObjectObj* suana2 = nullptr;
	//壁
	ObjectObj* kabe = nullptr;
	ObjectObj* kabe2 = nullptr;

	//天球
	ObjectObj* tenQ = nullptr;

	bool flag1 = false;
	bool clear = false;
	int time = 0;
	int cameraToMouse = 1;

	std::list<std::shared_ptr<EnemyZako>>enemiesG;

	PostEffect* postEffect = nullptr;

	Canvas* canvas = nullptr;

	static const int maxEnemy = 6;
	int index = 0;
	int timer[maxEnemy];

	int dasuteki[maxEnemy];
};

