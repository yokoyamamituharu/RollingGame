#pragma once
#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "InputMouse.h"
#include "PostEffect.h"
#include "GameCamera.h"
#include "Sprite.h"
#include "ObjectObj.h"
#include "ObjectFBX.h"
#include "FbxLoader.h"
#include "Player.h"
#include "EnemyZako.h"
#include "DefenseTower.h"
#include "BatlleScene.h"
#include <vector>
#include "PostEffect.h"
#include "Canvas.h"
#include "CopyObject.h"
#include "SceneLoader.h"
#include "CollisionManager.h"
#include "TouchableObject.h"
#include "DefenseTower.h"
class SceneManeger;
class BattleScene;

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

	void SetEnemy(std::shared_ptr<BaseEnemy>& enemy) {
		enemiesG.push_back(std::move(enemy));
	}

	//�|�X�g�G�t�F�N�g�̏���
	void PostReserve();

	//�Q�[���V�[���̃|�X�g�G�t�F�N�g��`��
	void PostDraw();

	//�G��������
	void SpownEnemy();

	void CreaEffect();

	void GameEndEffect();

public:
	Player* GetPlayer() { return player; }

private: // �����o�ϐ�
	/*---�|�C���^---*/
	DirectXCommon* dxCommon = nullptr;	

	CollisionManager* collisionManager = nullptr;

	/*----�I�u�W�F�N�g----*/
	//�X�v���C�g
	Sprite* spriteBG = nullptr;
	Sprite* clearsprite = nullptr;
	Canvas* canvas = nullptr;
	Sprite* pose = nullptr;
	//3D�I�u�W�F�N�g
	//�Q�[���V�[���p�I�u�W�F�N�g
	SceneLoader* scene = nullptr;
	//�^���[
	DefenseTower* defenseTower = nullptr;
	//�v���C���[
	Player* player = nullptr;	
	//�J����
	GameCamera* gameCamera = nullptr;
	//std::map<std::string, ObjectObj*> objects;
	//�G
	std::list<std::shared_ptr<BaseEnemy>>enemiesG;

	std::list<std::shared_ptr<DefenseTower>>towers;

	TouchableObject* kabe1 = nullptr;
	TouchableObject* kabe2 = nullptr;
	TouchableObject* kabe3 = nullptr;
	TouchableObject* kabe4 = nullptr;

	ObjectObj* castle = nullptr;
	ObjectObj* breakCastle = nullptr;
	ObjectObj* breaktenQ = nullptr;
	ObjectObj* breakGround = nullptr;
	int breakTimer = 0;


	bool isEvent = false;

	//�~�j�}�b�v�p�I�u�W�F�N�g
	CopyObject* copyGround = nullptr;
	CopyObject* copyCastle = nullptr;
	CopyObject* copyDefenseTower = nullptr;
	CopyObject* copyPlayer = nullptr;
	//�~�j�}�b�v�p�J����
	Camera* subCamera = nullptr;
	//�~�j�}�b�v�p�|�X�g�G�t�F�N�g
	PostEffect* miniMapPost = nullptr;
	bool mapFlag = false;	

	//�����̏ꏊ
	enum nestHole {
		HOLE1 = 1,
		HOLE2 = 2
	};
	//�G���o���܂ł̎��ԂƏꏊ
	struct Dasu {
		int timer;
		int hole;
	};

	static const int maxEnemy = 7;	
	std::map<int,Dasu>dasu;
	int index = 0;

	//�N���A�t���O
	bool clear = false;
	//�|�[�Y�t���O
	bool poseFlag = false;

	std::list<bool> tikaiStack;
	int tikaiTime = 0;
	bool isTikai = false;
	Sprite* tikaiSprite = nullptr;

	Sprite* playerSprte = nullptr;
	Sprite* towerSprte = nullptr;

	Particle* particleM = nullptr;

	DefenseTower* towera = nullptr;

	//�n�܂�
};

