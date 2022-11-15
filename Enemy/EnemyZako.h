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

	
public:	//�ÓI�����o�֐�
	//�G���s�������邩�����Ȃ���
	static void Action() {
		if (Input::GetInstance()->TriggerKey(DIK_E)) {
			isAction *= -1;
		}
	}
	static void EnemyCreateModel();


public:	//�����o�֐�
#pragma region �����o�֐�
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

	virtual void Initialize(int filedFlag, Camera* camera, XMFLOAT3 pos = { 0,0,0 }, bool isTarget = false, XMFLOAT3 targetPos1 = { 0,0,0 }, XMFLOAT3 targetPos2 = { 0,0,0 });
	virtual void Update();
	void Draw();

	void SetPlayer(Player* player)
	{
		this->player = player;
	}

	
#pragma endregion
public:
	void SetCamera(Camera* camera) { this->camera = camera; }

	//�v���C���[�̕����������鏈��
	void Direction(Player* player);


	std::list<std::unique_ptr<EnemyZako>>& GetEnemies() { return enemies; }

	float num = 0;

	XMFLOAT3 oldPos = { 0,0,0 };

public:
	OBJobject* object = nullptr;
	float GetHp() { return hp; }


	void Stop();

	void SetDead() { dead = true; }

	bool IsDead() { return dead; }

	bool GetAttack() { return attackFlag; }

protected:
	//�v���C���[�̕��ɕ��݊��֐�
	void DirectionWotch();
	//�ːi�O�̏���
	void Maeburi();

public:	//�ÓI�����o�ϐ�
	//�����Ă��̒n�ʂƂȂ�̍��W
	static const float groundOutPos;
	static const float groundInPos;
	static Model* enemyModel;
	//�G�̍s��������/�����Ȃ��t���O
	static int isAction;

protected:	//�����o�ϐ�
	//�|�C���^�[
	Camera* camera;
	Player* player;



	//�t���O
	bool atattckFlag = false;
	bool dead = false;
	bool nearFlag = false;

	int isFiled = FIELD_NONE;	//1:�O�V�[���A2:���V�[��

	bool stopFlag = false;		//�U���̏I���ɒ�~������
	bool animeEndFlag = false;
	
	bool maeburiFlag = false;	//�U���̑O�Ԃ�A�j���V���[���Đ��t���O
	bool tossinFlag = false;
	bool attackFlag = false;
	bool isTarget = false;		//�����������ɖړI�n���ݒ肳�ꂽ��

	

	//�^�C�}�[
	int jumpTime = 0;
	int maxStorpTime = 0;
	int stopTime = maxStorpTime;
	int waitTime = 0;
	int time = 0;
	int timer1 = 0;
	int tossintime = 0;
	int moveTime = 0;	
	int attackTime = 0;
	int rotaTime = 0;

	/*���W*/
	//�^�[�Q�b�g�̍��W
	XMVECTOR targetPos;
	//�^�[�Q�b�g�ɐi�ޕ���
	XMVECTOR targetVec;
	//�^�[�Q�b�g�̏ꏊ
	XMFLOAT3 targetPos1;
	XMFLOAT3 targetPos2;
	int targetIndex;//���܉��ڂ̃^�[�Q�b�g�Ȃ̂�

	/*���̑�*/
	XMFLOAT3 atodekesuROta = {0,0,0};
	int num1 = 0;
	int num2 = 0;
	XMFLOAT3 move;
	
	float angleToPlayer = 0.0f;

	float m_Radius;		// ���a(�`��p)
	float m_Angle = 0;	// �p�x
	float m_Length = 50;// ���a�̒���
	XMFLOAT3 rollPoint;
	
	XMVECTOR attackDirection;


	float maxHp = 100;
	float hp = maxHp;

	float scale = 1;
	XMFLOAT3 maxScale = { 4.0f,4.0f, 4.0f };


	std::list<std::unique_ptr<EnemyZako>>enemies;
};