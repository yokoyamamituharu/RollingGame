//#pragma once
//#include "ObjectObj.h"
//#include "Input.h"
//#include "InputMouse.h"
//#include <DirectXMath.h>
//#include "Player.h"
//#include "Enemy.h"
//class BaseEnemy
//{
//private:
//	//DirectX���ȗ�
//	using XMFLOAT2 = DirectX::XMFLOAT2;
//	using XMFLOAT3 = DirectX::XMFLOAT3;
//	using XMFLOAT4 = DirectX::XMFLOAT4;
//	using XMVECTOR = DirectX::XMVECTOR;
//	using XMMATRIX = DirectX::XMMATRIX;
//
//public:	//�T�u�N���X
//	enum FIELDLOCATION {
//		FIELD_NONE,
//		FIELD_OUT,
//		FIELD_IN
//	};
//
//public:	//�ÓI�����o�֐�
//	/// <summary>
//	/// �C���X�^���X�𐶐�
//	/// </summary>
//	/// <param name="filedFlag">�����t�B�[���h</param>
//	/// <param name="pos">�����ʒu</param>
//	/// <param name="isTarget">�ړI�n���ݒ肳��Ă��邩�ǂ���</param>
//	/// <returns>�G�N���X�̃C���X�^���X</returns>
//	static BaseEnemy* CreateIn(int filedFlag, XMFLOAT3 pos, bool isTarget);
//
//	/// <summary>
//	/// �V�[�����ɂ���G�̍s����S��~�A�������͓�����
//	/// </summary>
//	/// <param name="flag">�s�����邩�ۂ�</param>
//	static void Action(bool flag) {
//		isAction = flag;
//	}
//
//public:	//�ÓI�����o�ϐ�
//	//�����Ă��̒n�ʂƂȂ�̍��W
//	static const float groundOutPos;
//	static const float groundInPos;
//	//�G�̍s��������/�����Ȃ��t���O
//	static int isAction;
//
//public:	//�����o�֐�	
//
//	//�R���X�g���N�^
//	BaseEnemy();
//
//	//�f�X�g���N�^
//	~BaseEnemy();
//
//	/// <summary>
//	/// �O�V�[���̓G�̏���������
//	/// </summary>
//	/// <param name="pos">�����ʒu</param>
//	/// <param name="isTarget">�ړI�n���ݒ肳��Ă��邩�ǂ���</param>
//	/// <param name="targetPos1">�ړI�n2</param>
//	/// <param name="targetPos2">�ړI�n1</param>
//	virtual void InitializeOut(XMFLOAT3 pos, bool isTarget, XMFLOAT3 targetPos1 = { 0,0,0 }, XMFLOAT3 targetPos2 = { 0,0,0 });
//
//	/// <summary>
//	/// ���V�[���̓G�̏���������
//	/// </summary>
//	virtual void InitializeIn();
//
//	/// <summary>
//	/// �O�V�[���ł̍X�V����
//	/// </summary>
//	virtual void UpdateOut();
//
//	/// <summary>
//	/// ���V�[���ł̍X�V����
//	/// </summary>
//	virtual void UpdateIn();
//
//	/// <summary>
//	/// �G�̕`��
//	/// </summary>
//	void Draw();
//
//protected:
//	/// <summary>
//	/// �v���C���[�̕��������鏈��
//	/// </summary>
//	/// <param name="player"></param>
//	void ViewpointPlayer(Player* player);
//
//	/// <summary>
//	/// �v���C���[�̕����֐i�ޏ���
//	/// </summary>
//	void ApproachPlayer();
//
//	/// <summary>
//	///	�U���O�̗\������
//	/// </summary>
//	void PreliminaryOperation();
//
//	/// <summary>
//	/// ���S���̃p�[�e�B�N�����p�[�e�B�N���N���X�ɒǉ�
//	/// </summary>
//	void BreakParticle();
//
//
//public:	//�Z�b�^�[�A�Q�b�^�[	
//
//	/// <summary>
//	/// �_���[�W���󂯂鏈��
//	/// </summary>
//	/// <param name="attackPower">�󂯂�_���[�W�l</param>
//	void Damege(int damage);
//
//	/// <summary>
//	/// ���g�̍s�����~�߂�
//	/// </summary>
//	void Stop();
//
//	/// <summary>
//	/// �U���Ώۂ̃v���C���[���Z�b�g����
//	/// </summary>
//	/// <param name="player"></param>
//	void SetPlayer(Player* player)
//	{
//		this->player = player;
//	}
//
//	/// <summary>
//	/// ���S�����t����
//	/// </summary>
//	void SetDead() { dead = true; }
//
//	/// <summary>
//	/// ���S������O��
//	/// </summary>
//	void NotDead() { dead = false; }
//
//	/// <summary>
//	/// �G�̎��S������Ƃ�
//	/// </summary>
//	/// <returns>����ł���true</returns>
//	bool GetDead() { return dead; }
//
//	/// <summary>
//	/// �U����������
//	/// </summary>
//	/// <returns>�U������������Ă��邩�ۂ�</returns>
//	bool GetAttack() { return attackFlag; }
//
//	/// <summary>
//	/// ���C�t�̒l���擾
//	/// </summary>
//	/// <returns>���C�t�̒l</returns>
//	float GetHp() { return hp; }
//
//	/// <summary>
//	/// �O�V�[���p�̓G�������Ă��钆�V�[���p�̓G�̃��X�g���擾
//	/// </summary>
//	/// <returns>�G�N���X�̃��X�g</returns>
//	std::list<std::unique_ptr<BaseEnemy>>& GetEnemies() { return InEnemies; }
//
//	/// <summary>
//	/// �p�[�e�B�N���𐶐�
//	/// </summary>
//	void ParticleCreate();
//
//public:
//	ObjectObj* object = nullptr;	//�I�u�W�F�N�g
//	XMFLOAT3 oldPos = { 0,0,0 };	//1�t���[���O�̃|�W�V����
//
//protected:	//�����o�ϐ�
//	//�|�C���^�[
//	Player* player;		//�v���C���[�̃|�C���^�[
//
//	//�ۉe
//	ObjectObj* shadowObj = nullptr;		//�ۉe
//
//	//�t���O
//	bool atattckFlag = false;	//�U�������ǂ���
//	bool dead = false;			//�u�]���Ă��邩�ǂ���
//	bool nearFlag = false;		//�v���C���[�ɋ߂����ǂ����i�������锻��ɂ͂����Ă��邩�ǂ����̃t���O�j
//
//	int isFiled = FIELD_NONE;	//1:�O�V�[���A2:���V�[��
//
//	bool stopFlag = false;		//�U���̏I���ɒ�~������
//	bool animeEndFlag = false;
//
//	bool maeburiFlag = false;	//�U���̑O�Ԃ�A�j���V���[���Đ��t���O
//	bool tossinFlag = false;	//�ːi�U��������
//	bool attackFlag = false;	//�U����
//	bool isTarget = false;		//�����������ɖړI�n���ݒ肳�ꂽ��
//
//
//
//	//�^�C�}�[
//	int jumpTime = 0;
//	int maxStorpTime = 0;
//	int stopTime = maxStorpTime;
//	int waitTime = 0;
//	int time = 0;
//	int timer1 = 0;
//	int tossintime = 0;
//	int moveTime = 0;
//	int attackTime = 0;
//	int rotaTime = 0;
//
//	/*���W*/
//	//�^�[�Q�b�g�ɐi�ޕ���
//	XMVECTOR targetVec;
//	//�^�[�Q�b�g�̏ꏊ
//	XMFLOAT3 targetPos1;
//	XMFLOAT3 targetPos2;
//	int targetIndex;//���܉��ڂ̃^�[�Q�b�g�Ȃ̂�
//
//	/*���̑�*/
//	XMFLOAT3 atodekesuROta = { 0,0,0 };
//	int num1 = 0;
//	int num2 = 0;
//	XMFLOAT3 move;
//
//
//	float m_Radius;		// ���a(�`��p)
//	float m_Angle = 0;	// �p�x
//	float m_Length = 50;// ���a�̒���
//	XMFLOAT3 rollPoint;
//
//	
//	XMVECTOR attackDirection;	//�ːi�������
//
//
//	float maxHp = 100;
//	float hp = maxHp;
//
//	float scale = 1;
//	XMFLOAT3 maxScale = { 4.0f,4.0f, 4.0f };
//
//
//	std::list<std::unique_ptr<BaseEnemy>>InEnemies;
//};