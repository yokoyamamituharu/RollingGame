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
	//�R���X�g���N�^
	BatlleScene();
	//�f�X�g���N�^
	~BatlleScene();
	//������
	void Initialize(DirectXCommon* dxCommon);
	//�X�V
	void Update(int& sceneNo, GameScene* gameScene);
	//�`��
	void Draw();

	void SetEnemies(std::shared_ptr<EnemyZako>& enemy) {
		enemy1 = std::move(enemy);
	}

private: // �����o�ϐ�        
	/*----�|�C���^----*/
	DirectXCommon* dxCommon = nullptr;

	/*----�I�u�W�F�N�g----*/
	//�X�v���C�g
	Sprite* spriteBG = nullptr;
	Canvas* canvas = nullptr;
	//3D�I�u�W�F�N�g
	ObjectObj* tenQ = nullptr;
	ObjectObj* ground = nullptr;	
	Player* player = nullptr;
	std::shared_ptr<EnemyZako>enemy1;
	ObjectObj* area = nullptr;

	//�J����
	BattleCamera* battleCamera = nullptr;
};