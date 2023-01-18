#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "InputMouse.h"
#include "PostEffect.h"
#include "Camera.h"
#include "Sprite.h"
#include "ObjectObj.h"
#include "ObjectFBX.h"
#include "FbxLoader.h"
#include "Player.h"
#include "EnemyZako.h"
#include "Canvas.h"
#include "BattleCamera.h"

#include <vector>
class GameScene;

class BatlleScene
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
	BatlleScene();
	//デストラクタ
	~BatlleScene();
	//初期化
	void Initialize(DirectXCommon* dxCommon);
	//更新
	void Update(int& sceneNo, GameScene* gameScene);

	//3D空間上の更新
	void Update3D(int& sceneNo, GameScene* gameScene);

	//2D空間上の更新
	void Update2D();

	//描画
	void Draw();

	void SetEnemies(std::shared_ptr<EnemyZako>& enemy) {
		enemies = std::move(enemy);
	}

private: // メンバ変数        
	/*----ポインタ----*/
	DirectXCommon* dxCommon = nullptr;

	/*----オブジェクト----*/
	//スプライト
	int hitNum;
	int hitTime;
	const int hitWaitTime = 90;	//ヒットが継続する猶予時間
	bool hitFlag = false;

	Sprite* hitNum1[10] = { nullptr };
	Sprite* hitNum2[10] = { nullptr };
	Sprite* hitSprite = nullptr;
	Sprite* mousePosS = nullptr;

	float hitSize = 1.0f;
	XMFLOAT2 hitSizeB = { 1.0f ,1.0f};

	bool hitStopFlag = false;
	int hitStopTime = 0;

	Sprite* torisetu = nullptr;
	Canvas* canvas = nullptr;
	//3Dオブジェクト
	ObjectObj* tenQ = nullptr;
	ObjectObj* ground = nullptr;
	Player* player = nullptr;
	std::shared_ptr<EnemyZako>enemies;
	ObjectObj* area = nullptr;

	//カメラ
	BattleCamera* battleCamera = nullptr;
};