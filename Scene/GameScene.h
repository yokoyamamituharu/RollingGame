#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "InputMouse.h"
#include "PostEffect.h"
#include "Camera.h"
#include "Sprite.h"
#include "OBJobject.h"
#include "FBXObject.h"
#include "FbxLoader.h"
#include "Player.h"
#include "EnemyZako.h"
#include "DefenseTower.h"
#include "Enemy.h"
#include "BatlleScene.h"
class BattleScene;
class SceneManeger;
#include <vector>

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
	void Initialize(DirectXCommon* dxCommon,Input* input, InputMouse* mouse,Camera* camera);
    //更新
	void Update(int& sceneNo, BatlleScene* batlleScene);
    //描画
	void Draw();

public:
    Player* GetPlayer() { return player; }

private: // メンバ変数
    DirectXCommon* dxCommon = nullptr;
    Input* input = nullptr;
    InputMouse* mouse = nullptr;
    Camera* camera;

    // ゲームシーン用  
    //スプライト
    Sprite* spriteBG = nullptr;
    Sprite* clearsprite = nullptr;
    //3Dオブジェクト
    FbxModel* fbxmodel = nullptr;
    FBXObject* fbxobject = nullptr;

    //プレイヤー
    Player* player = nullptr;
    //地面
    Model* groundmodel = nullptr;
    OBJobject* ground = nullptr;
    //敵
    Model* enemymodel = nullptr;
    //タワー
    DefenseTower* defenseTower = nullptr;
    Bullet* bullet = nullptr;
    EnemyZako* enemy1 = nullptr;
    //お城
    Model* castleModel = nullptr;
    OBJobject *castle = nullptr;

    bool flag1 = false;
    bool clear = false;
    int time = 0;
    int cameraToMouse = 1;
};

