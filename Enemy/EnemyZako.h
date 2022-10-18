#pragma once
#include "OBJObject.h"
#include "Input.h"
#include "InputMouse.h"
#include <DirectXMath.h>
#include "Player.h"
class EnemyZako
{
private:
	//DirectX���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

	float Nitenkan(XMFLOAT3 pos1, XMFLOAT3 pos2);
public:
	void Initialize(Input* input, InputMouse* mouse, Camera* camera);
	void Update();
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

public:
	void SetInput(Input* input) { this->input = input; }
	void SetMouse(InputMouse* mouse) { this->mouse = mouse; }
	void SetCamera(Camera* camera) { this->camera = camera; }

	//�^�[�Q�b�g�Ɍ������Ă܂������s������
	void GoTarget(XMFLOAT3 target);

	//�v���C���[�̕����������鏈��
	void Direction(Player* player);

	//�v���C���[���܂�肱�ނ悤�Ɉړ����鏈��
	void Mawarikomi(Player* player);

	float num = 0;

public:
	OBJobject* object = nullptr;

	bool flag1 = false;
private:
	Model* enemyModel = nullptr;
	Input* input;
	InputMouse* mouse;

	Camera* camera;

	XMFLOAT2 targetpos;
	bool atattckFlag = false;


	//Player *player;
	float angle = 0.0f;


	XMFLOAT3 enemypos;
	XMFLOAT3 enemypos1;

	//XMFLOAT3 oldPlayerPos;
	XMFLOAT3 targetPos;

	int waitTime = 0;
	int time = 0;

	Player* player;

	int num1 = 0;
	int num2 = 0;
	XMFLOAT3 move;

	int timer1 = 0;


	float m_PosX;		// �`����WX
	float m_PosY;		// �`����WY
	float m_Radius;		// ���a(�`��p)
	float m_CenterX;	// ���S���WX
	float m_CenterY;	// ���S���WY
	float m_Angle = 0;		// �p�x
	float m_Length = 50;		// ���a�̒���

	int tossintime = 0;
	bool tossinFlag = false;

	int flag = 1;

	int moveTime = 0;
	bool attackFlag = false;
	int attackTime = 0;
	XMFLOAT3 attackPoint;
	bool attackFlag2 = false;
	XMVECTOR direction1;

	bool nearFlag = false;
	XMFLOAT3 rollPoint;
};