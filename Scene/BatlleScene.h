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
#include "../Canvas.h"

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
	//描画
	void Draw();

	void SetEnemies(std::shared_ptr<EnemyZako>& enemy) {
		enemy1 = std::move(enemy);
	}

private: // メンバ変数        
	//ポインター
	DirectXCommon* dxCommon = nullptr;	

	/*----オブジェクト----*/
	//3Dオブジェクト
	Player* player = nullptr;
	std::shared_ptr<EnemyZako>enemy1;
	ObjectObj* ground = nullptr;
	ObjectObj* tenQ = nullptr;
	//スプライト
	Sprite* spriteBG = nullptr;	
	Canvas* canvas = nullptr;

	//カメラ
	GameCamera* gameCamera = nullptr;
	
	int cameraToMouse = 1;
};