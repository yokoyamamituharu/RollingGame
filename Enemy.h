#pragma once
#include "OBJObject.h"
#include "Input.h"
#include "InputMouse.h"
#include <DirectXMath.h>
#include "Player.h"
class Enemy
{
private:
	//DirectX���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;


public:
	void Initialize(Input* input, InputMouse* mouse, Camera* camera);
	void Update();
	void Move();
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

public:
	void SetInput(Input* input) { this->input = input; }
	void SetMouse(InputMouse* mouse) { this->mouse = mouse; }
	void SetCamera(Camera* camera) { this->camera = camera; }

	//�^�[�Q�b�g�Ɍ������Ă܂������s������
	void GoTarget(Player* player);

	//�v���C���[�̕����������鏈��
	void Direction(Player *player);

	//�v���C���[���܂�肱�ނ悤�Ɉړ����鏈��
	void Mawarikomi(Player* player);

	float num = 0;

public:
	OBJobject* object = nullptr;

	bool attackFlag = false;

private:
	Input* input;
	InputMouse* mouse;
	Model* playermodel = nullptr;
	Model* playerSpheremodel = nullptr;

	bool isSphere = false;
	DirectX::XMFLOAT3 spiralSpeed = {};
	float rollingSpeed = 0.0f;

	Camera* camera;

	XMFLOAT2 targetpos;
	bool atattckFlag = false;


	//Player *player;
	float angle = 0.0f;

	float speed = 5.0f;

	XMFLOAT3 enemypos;
	XMFLOAT3 enemypos1;

	XMFLOAT3 oldPlayerPos;

	int time = 0;
};