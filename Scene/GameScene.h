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
#include "Enemy.h"
#include "BatlleScene.h"
class BattleScene;
class SceneManeger;
#include <vector>
#include "PostEffect.h"
#include "Canvas.h"
#include "CopyObject.h"
#include "SceneLoader.h"
#include "CollisionManager.h"
#include "TouchableObject.h"

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
	/*�|�C���^*/
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
	//�V��
	ObjectObj* tenQ = nullptr;
	//�n��
	ObjectObj* ground = nullptr;	
	//����
	ObjectObj* castle = nullptr;
	TouchableObject* touchCastle = nullptr;
	//����
	ObjectObj* suana = nullptr;
	ObjectObj* suana2 = nullptr;
	//��
	ObjectObj* kabe = nullptr;
	ObjectObj* kabe2 = nullptr;
	//�^���[
	DefenseTower* defenseTower = nullptr;
	//�v���C���[
	Player* player = nullptr;	
	//�J����
	GameCamera* gameCamera = nullptr;
	//std::map<std::string, ObjectObj*> objects;
	//�G
	std::list<std::shared_ptr<EnemyZako>>enemiesG;
	TouchableObject* touchGround = nullptr;

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

	//�G���o�����ԂƏꏊ
	struct Dasu {
		int timer;
		int basyo;
	};
	static const int maxEnemy = 6;	
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
};

