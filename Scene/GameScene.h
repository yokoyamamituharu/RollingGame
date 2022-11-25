#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "InputMouse.h"
#include "PostEffect.h"
#include "GameCamera.h"
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
#include "Canvas.h"
#include "CopyObject.h"

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
	void Initialize(DirectXCommon* dxCommon);
	//�X�V
	void Update(int& sceneNo, BatlleScene* batlleScene);
	//�`��
	void Draw();

	void SetEnemy(std::shared_ptr<EnemyZako>& enemy) {
		enemiesG.push_back(std::move(enemy));
	}

	//�|�X�g�G�t�F�N�g�̏���
	void PostReserve();

	//�Q�[���V�[���̃|�X�g�G�t�F�N�g��`��
	void PostDraw();

public:
	Player* GetPlayer() { return player; }

private: // �����o�ϐ�
	DirectXCommon* dxCommon = nullptr;
	GameCamera* mainCamera = nullptr;

	// �Q�[���V�[���p  
	//�X�v���C�g
	Sprite* spriteBG = nullptr;
	Sprite* clearsprite = nullptr;
	//3D�I�u�W�F�N�g
	FbxModel* fbxmodel = nullptr;
	ObjectFBX* objectFBX = nullptr;

	//�v���C���[
	Player* player = nullptr;
	//�n��
	ObjectObj* ground = nullptr;
	//�G
	//�^���[
	DefenseTower* defenseTower = nullptr;
	//EnemyZako* enemy1 = nullptr;
	//����
	ObjectObj* castle = nullptr;
	//����
	ObjectObj* suana = nullptr;
	ObjectObj* suana2 = nullptr;
	//��
	ObjectObj* kabe = nullptr;
	ObjectObj* kabe2 = nullptr;
	//�V��
	ObjectObj* tenQ = nullptr;

	//std::map<std::string, ObjectObj*> objects;

	CopyObject* copyPlayer = nullptr;
	CopyObject* copyGround = nullptr;
	CopyObject* copyCastle = nullptr;
	CopyObject* copyDefenseTower = nullptr;
	//CopyObject* copyGround = nullptr;

	bool flag1 = false;
	bool clear = false;
	int time = 0;

	Camera* subCamera = nullptr;

	std::list<std::shared_ptr<EnemyZako>>enemiesG;

	PostEffect* postEffect = nullptr;

	Canvas* canvas = nullptr;

	static const int maxEnemy = 6;
	int index = 0;
	int timer[maxEnemy];

	int dasuteki[maxEnemy];

	bool mapFlag = false;
};

