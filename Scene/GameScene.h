#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "InputMouse.h"
#include "PostEffect.h"
#include "GameCamera.h"
#include "Sprite.h"
#include "ObjectObj.h"
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
#include "Canvas.h"
#include "CopyObject.h"
#include "SceneLoader.h"
#include "CollisionManager.h"
#include "TouchableObject.h"

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
	void Initialize(DirectXCommon* dxCommon);
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
	/*ポインタ*/
	DirectXCommon* dxCommon = nullptr;	

	CollisionManager* collisionManager = nullptr;

	/*----オブジェクト----*/
	//スプライト
	Sprite* spriteBG = nullptr;
	Sprite* clearsprite = nullptr;
	Canvas* canvas = nullptr;
	Sprite* pose = nullptr;
	//3Dオブジェクト
	//ゲームシーン用オブジェクト
	SceneLoader* scene = nullptr;
	//天球
	ObjectObj* tenQ = nullptr;
	//地面
	ObjectObj* ground = nullptr;	
	//お城
	ObjectObj* castle = nullptr;
	TouchableObject* touchCastle = nullptr;
	//巣穴
	ObjectObj* suana = nullptr;
	ObjectObj* suana2 = nullptr;
	//壁
	ObjectObj* kabe = nullptr;
	ObjectObj* kabe2 = nullptr;
	//タワー
	DefenseTower* defenseTower = nullptr;
	//プレイヤー
	Player* player = nullptr;	
	//カメラ
	GameCamera* gameCamera = nullptr;
	//std::map<std::string, ObjectObj*> objects;
	//敵
	std::list<std::shared_ptr<EnemyZako>>enemiesG;
	TouchableObject* touchGround = nullptr;

	//ミニマップ用オブジェクト
	CopyObject* copyGround = nullptr;
	CopyObject* copyCastle = nullptr;
	CopyObject* copyDefenseTower = nullptr;
	CopyObject* copyPlayer = nullptr;
	//ミニマップ用カメラ
	Camera* subCamera = nullptr;
	//ミニマップ用ポストエフェクト
	PostEffect* miniMapPost = nullptr;
	bool mapFlag = false;	

	//敵を出す時間と場所
	struct Dasu {
		int timer;
		int basyo;
	};
	static const int maxEnemy = 6;	
	std::map<int,Dasu>dasu;
	int index = 0;

	//クリアフラグ
	bool clear = false;
	//ポーズフラグ
	bool poseFlag = false;

	std::list<bool> tikaiStack;
	int tikaiTime = 0;
	bool isTikai = false;
	Sprite* tikaiSprite = nullptr;
};

