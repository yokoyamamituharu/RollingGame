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
#include "PostEffect.h"
#include "../Canvas.h"

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
	void Initialize(DirectXCommon* dxCommon, Input* input, InputMouse* mouse, Camera* camera);
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

	//�v���C���[
	Player* player = nullptr;
	//�n��
	Model* groundmodel = nullptr;
	OBJobject* ground = nullptr;
	//�G
	Model* enemymodel = nullptr;
	//�^���[
	DefenseTower* defenseTower = nullptr;
	Bullet* bullet = nullptr;
	//EnemyZako* enemy1 = nullptr;
	//����
	Model* castleModel = nullptr;
	OBJobject* castle = nullptr;

	//����
	Model* suanaModel = nullptr;
	OBJobject* suana = nullptr;
	OBJobject* suana2 = nullptr;
	//��
	Model* kabeModel = nullptr;
	OBJobject* kabe = nullptr;
	OBJobject* kabe2 = nullptr;

	//�V��
	Model* tenqModel = nullptr;
	OBJobject* tenQ = nullptr;

	bool flag1 = false;
	bool clear = false;
	int time = 0;
	int cameraToMouse = -1;

	std::list<std::shared_ptr<EnemyZako>>enemiesG;

	PostEffect* postEffect = nullptr;

	Canvas* canvas = nullptr;

};

