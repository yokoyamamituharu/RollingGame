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
#include "Enemy.h"

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
	void Update();
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

    Model* playermodel = nullptr;
    Model* playerSpheremodel = nullptr;
    Player* player = nullptr;
    Model* groundmodel = nullptr;
    OBJobject* ground = nullptr;

    Model* enemymodel = nullptr;
    OBJobject* enemy = nullptr;

    Enemy* enemys[6] = { nullptr };


    bool flag1 = false;

    bool alive[6] = { true };
    bool clear = false;

    int time = 0;
};

