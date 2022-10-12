#pragma once
#include "OBJObject.h"
#include "Input.h"
#include "InputMouse.h"
#include <DirectXMath.h>
#include "Player.h"
class EnemyZako
{
private:
	//DirectXを省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;


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

	//ターゲットに向かってまっすぐ行く処理
	void GoTarget(XMFLOAT3 target);

	//プレイヤーの方を向かせる処理
	void Direction(Player* player);

	//プレイヤーをまわりこむように移動する処理
	void Mawarikomi(Player* player);

	float num = 0;

public:
	OBJobject* object = nullptr;

	bool attackFlag = false;
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
};