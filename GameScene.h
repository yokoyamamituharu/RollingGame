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
    //�R���X�g���N�^
    GameScene();
    //�f�X�g���N�^
    ~GameScene();
    //������
	void Initialize(DirectXCommon* dxCommon,Input* input, InputMouse* mouse,Camera* camera);
    //�X�V
	void Update();
    //�`��
	void Draw();

public:
    Player* GetPlayer() { return player; }

private: // �����o�ϐ�
    DirectXCommon* dxCommon = nullptr;
    Input* input = nullptr;
    InputMouse* mouse = nullptr;
    Camera* camera;

    // �Q�[���V�[���p  
    //�X�v���C�g
    Sprite* spriteBG = nullptr;
    Sprite* clearsprite = nullptr;
    //3D�I�u�W�F�N�g
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

