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
        //�R���X�g���N�^
        BatlleScene();
        //�f�X�g���N�^
        ~BatlleScene();
        //������
        void Initialize(DirectXCommon* dxCommon, Input* input, InputMouse* mouse, Camera* camera,GameScene* gameScene);
        //�X�V
        void Update(int& sceneNo, GameScene* gameScene);
        //�`��
        void Draw();

        void SetEnemies(std::shared_ptr<EnemyZako>&enemy) {
            enemy1 = std::move(enemy);
        }

    private: // �����o�ϐ�
        DirectXCommon* dxCommon = nullptr;
        Input* input = nullptr;
        InputMouse* mouse = nullptr;
        Camera* camera;

        // �Q�[���V�[���p  
        //�X�v���C�g
        Sprite* spriteBG = nullptr;
        Sprite* claerSprite = nullptr;
        //3D�I�u�W�F�N�g
        FbxModel* fbxmodel = nullptr;
        FBXObject* fbxobject = nullptr;

        Model* playermodel = nullptr;
        Model* playerSpheremodel = nullptr;
        Player* player = nullptr;
        Model* groundmodel = nullptr;
        OBJobject* ground = nullptr;
        Model* tenQQModel = nullptr;
        OBJobject* tenQ = nullptr;

        Model* enemymodel = nullptr;

        //EnemyZako* enemys[6] = { nullptr };

        std::shared_ptr<EnemyZako>enemy1;

        //
        EnemyZako* parentEnemy = nullptr;
        Canvas* canvas = nullptr;

        bool alive[6];
        int time = 0;
        int cameraToMouse = 1;
    };

