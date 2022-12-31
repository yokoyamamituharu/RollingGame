#pragma once
#include "ObjectObj.h"
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

	enum FIELDLOCATION {
		FIELD_NONE,
		FIELD_OUT,
		FIELD_IN
	};

	static EnemyZako* CreateIn(int filedFlag, XMFLOAT3 pos,bool isTarget);

public:	//�ÓI�����o�ϐ�
//�����Ă��̒n�ʂƂȂ�̍��W
	static const float groundOutPos;
	static const float groundInPos;
	//�G�̍s��������/�����Ȃ��t���O
	static int isAction;

public:	//�����o�֐�	

	//�R���X�g���N�^
	EnemyZako();

	//�f�X�g���N�^
	~EnemyZako();	

	virtual void InitializeOut( XMFLOAT3 pos, bool isTarget, XMFLOAT3 targetPos1 = { 0,0,0 }, XMFLOAT3 targetPos2 = { 0,0,0 });

	virtual void InitializeIn();

	virtual void UpdateOut();

	virtual void UpdateIn();

	void Draw();
	
protected:
	//�v���C���[�̕����������鏈��
	void ViewpointPlayer(Player* player);

	//�v���C���[�̕��ɕ��݊��֐�
	void ApproachPlayer();

	//�ːi�O�̗\������
	void PreliminaryOperation();
	
	void BreakParticle();


public:	//�Z�b�^�[�A�Q�b�^�[	
	void Damege(int attackPower);
	
	void SetPlayer(Player* player)
	{
		this->player = player;
	}
	void Stop();

	void SetDead() { dead = true; }

	void NotDead() { dead = false; }

	/// <summary>
	/// �G�̎��S������Ƃ�
	/// </summary>
	/// <returns>����ł���true</returns>
	bool GetDead() { return dead; }

	bool GetAttack() { return attackFlag; }

	float GetHp() { return hp; }

	std::list<std::unique_ptr<EnemyZako>>& GetEnemies() { return enemies; }

	void ParticleCreate();

public:
	ObjectObj* object = nullptr;
	XMFLOAT3 oldPos = { 0,0,0 };

protected:	//�����o�ϐ�
	//�|�C���^�[
	Player* player;

	//�ۉe
	ObjectObj* shadowObj = nullptr;

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