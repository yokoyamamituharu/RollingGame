#pragma once
#include "OBJObject.h"
#include "Input.h"
#include "InputMouse.h"
#include <DirectXMath.h>
#include "Player.h"
#include "Enemy.h"
class EnemyZako
{
private:
	//DirectX���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

	


public:
	float Nitenkan(XMFLOAT3 pos1, XMFLOAT3 pos2);

	enum FIELDLOCATION {
		FIELD_NONE,
		FIELD_OUT,
		FIELD_IN
	};


	//�R���X�g���N�^
	EnemyZako();

	//�f�X�g���N�^
	~EnemyZako();

	void Damege(int attackPower) { hp -= attackPower; }

	virtual void Initialize(int filedFlag, Camera* camera, XMFLOAT3 pos = { 0,0,0 }, bool isTarget = false, XMFLOAT3 targetPos = { 0,0,0 });
	virtual void Update();
	void Draw();

	void MoveVector(DirectX::XMFLOAT3 vec)
	{
		object->SetPosition({
		 object->GetPosition().x + vec.x,
		 object->GetPosition().y + vec.y,
		 object->GetPosition().z + vec.z });
	}

	void SpiralVector(DirectX::XMFLOAT3 vec)
	{
		object->SetRotation({
		 object->GetRotation().x + vec.x,
		 object->GetRotation().y + vec.y,
		 object->GetRotation().z + vec.z });
	}

	void SetPlayer(Player* player)
	{
		this->player = player;
	}

	//�G���s�������邩�����Ȃ���
	static void Action() {		
		if (Input::GetInstance()->TriggerKey(DIK_E)) {
			isAction *= -1;
		}
	}

public:
	void SetCamera(Camera* camera) { this->camera = camera; }

	//�^�[�Q�b�g�Ɍ������Ă܂������s������
	void GoTarget(XMFLOAT3 target);

	//�v���C���[�̕����������鏈��
	void Direction(Player* player);

	//�v���C���[���܂�肱�ނ悤�Ɉړ����鏈��
	void Mawarikomi(Player* player);

	std::list<std::unique_ptr<EnemyZako>>& GetEnemies() { return enemies; }

	float num = 0;

public:
	OBJobject* object = nullptr;
	float GetHp() { return hp; }

	static void EnemyCreateModel();

	void Stop();

	void SetDead() { dead = true; }

	bool IsDead() { return dead; }

	bool GetAttack() { return attackFlag; }

protected:
	//�v���C���[�̕��ɕ��݊��֐�
	void DirectionWotch();

	int maxStorpTime = 0;
	int stopTime = maxStorpTime;
	bool stopFlag = false;
	int jumpTime = 0;
	//�ːi�O�̏���
	void Maeburi();

protected:
	static Model* enemyModel;

	Camera* camera;

	bool atattckFlag = false;

	bool dead = false;

	XMFLOAT3 atodekesuROta = {0,0,0};
	//Player *player;
	float angle = 0.0f;

	XMFLOAT3 enemypos;
	XMFLOAT3 enemypos1;

	//XMFLOAT3 oldPlayerPos;
	XMVECTOR targetPos;
	//�^�[�Q�b�g�ɐi�ޕ���
	XMVECTOR targetVec;
	//XMFLOAT3 targetPos;

	int waitTime = 0;
	int time = 0;


	Player* player;

	int num1 = 0;
	int num2 = 0;
	XMFLOAT3 move;

	int timer1 = 0;


	float m_Radius;		// ���a(�`��p)
	float m_Angle = 0;	// �p�x
	float m_Length = 50;// ���a�̒���

	int tossintime = 0;
	bool tossinFlag = false;

	//�G�̍s��������/�����Ȃ��t���O
	static int isAction;

	int moveTime = 0;
	bool attackFlag = false;
	int attackTime = 0;
	XMVECTOR attackDirection;

	bool nearFlag = false;
	XMFLOAT3 rollPoint;

	int rotaTime = 0;

	std::list<std::unique_ptr<EnemyZako>>enemies;

	int isFiled = FIELD_NONE;	//1:�O�V�[���A2:���V�[��
	bool isTarget = false;	//�ړI�n�������������ɐݒ肳�ꂽ��



	float maxHp = 100;
	float hp = maxHp;

	float scale = 1;
	XMFLOAT3 maxScale = { 4.0f,4.0f, 4.0f };


	bool animeEndFlag = false;

	//�U���̑O�Ԃ�A�j���V���[���Đ��t���O
	bool maeburiFlag = false;

public:
	//�����Ă��̒n�ʂƂȂ�̍��W
	static const float groundOutPos;
	static const float groundInPos;
};