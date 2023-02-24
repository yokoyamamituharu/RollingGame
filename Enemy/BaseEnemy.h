#pragma once
#include "ObjectObj.h"
#include "Input.h"
#include "InputMouse.h"
#include <DirectXMath.h>
#include "Player.h"
class BaseEnemy
{
protected:
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

public:	//�ÓI�����o�ϐ�
	//�G�̒n�ʂƂȂ�̍��W
	static const float groundOutPos;
	static const float groundInPos;
	//�G�̍s��������/�����Ȃ��t���O
	static int isAction;

public:	//�����o�֐�	

	//�R���X�g���N�^
	BaseEnemy();

	//�f�X�g���N�^
	~BaseEnemy();

	virtual void InitializeOut(XMFLOAT3 pos, bool isTarget, XMFLOAT3 targetPos1 = { 0,0,0 }, XMFLOAT3 targetPos2 = { 0,0,0 });

	virtual void InitializeIn();

	virtual void UpdateOut();

	virtual void UpdateIn();

	virtual void Draw();

protected:
	//�v���C���[�̕����������鏈��
	void ViewpointPlayer(Player* player);

	//�v���C���[�̕��ɕ��݊��֐�
	void ApproachPlayer();

	//�ːi�O�̗\������
	void PreliminaryOperation();

	void BreakParticle();


public:	//�Z�b�^�[�A�Q�b�^�[

	/// <summary>
	/// �O�V�[���Ń_���[�W���󂯂�Ƃ��̏���
	/// </summary>
	/// <param name="damage">�󂯂�_���[�W��</param>
	void DamageOut(int damage);

	/// <summary>
	/// ���V�[���Ń_���[�W���󂯂�Ƃ��̏���
	/// </summary>
	/// <param name="damage">�󂯂�_���[�W��</param>
	void DamageIn(int damage);

	/// <summary>
	/// �v���C���[���Z�b�g���鏈��
	/// </summary>
	/// <param name="player">�v���C���[�̃A�h���X</param>
	void SetPlayer(Player* player)
	{
		this->player = player;
	}

	/// <summary>
	/// �U�������㗧���~�܂鎞�Ԃ��v�Z���鏈��
	/// </summary>
	void Stop();

	/// <summary>
	/// ���S�t���O���Z�b�g
	/// </summary>
	void SetDead() { dead = true; }

	/// <summary>
	/// ���S�t���O���O��
	/// </summary>
	void NotDead() { dead = false; }

	/// <summary>
	/// ���S������擾
	/// </summary>
	/// <returns>���S�t���O</returns>
	bool GetDead() { return dead; }

	/// <summary>
	/// �U��������擾
	/// </summary>
	/// <returns>�U������</returns>
	bool GetAttack() { return attackFlag; }

	/// <summary>
	/// HP���擾
	/// </summary>
	/// <returns>HP</returns>
	float GetHp() { return outhp; }


	void ParticleCreate();



public:
	ObjectObj* object = nullptr;
	XMFLOAT3 oldPos = { 0,0,0 };
	std::list<std::unique_ptr<BaseEnemy>>& GetEnemies() { return enemies; }


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
	XMFLOAT3 atodekesuROta = { 0,0,0 };
	int num1 = 0;
	int num2 = 0;
	XMFLOAT3 move;


	float m_Radius;		// ���a(�`��p)
	float m_Angle = 0;	// �p�x
	float m_Length = 50;// ���a�̒���
	XMFLOAT3 rollPoint;

	XMVECTOR attackDirection;


	float outmaxHp = 1;
	float outhp = outmaxHp;

	float inmaxHp = 1;
	float inhp = inmaxHp;

	float scale = 1;
	XMFLOAT3 maxScale = { 4.0f,4.0f, 4.0f };


	std::list<std::unique_ptr<BaseEnemy>>enemies;

	//���p�̓G���O�V�[���Ń}�b�v�����蔻��ɂЂ�������Ȃ��悤��
	XMFLOAT3 outPos = { 0,-100,0 };
	XMFLOAT3 inPos = { 0,-100,0 };

public:
	bool tikai = false;

};