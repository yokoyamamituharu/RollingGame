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
    //3D�I�u�W�F�N�g
    FbxModel* fbxmodel = nullptr;
    FBXObject* fbxobject = nullptr;

    Model* playermodel = nullptr;
    Model* playerSpheremodel = nullptr;
    Player* player = nullptr;
    Model* grundmodel = nullptr;
    OBJobject* ground = nullptr;
};
