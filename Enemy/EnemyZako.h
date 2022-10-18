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

	//ターゲットに向かってまっすぐ行く処理
	void GoTarget(XMFLOAT3 target);

	//プレイヤーの方を向かせる処理
	void Direction(Player* player);

	//プレイヤーをまわりこむように移動する処理
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


	float m_PosX;		// 描画座標X
	float m_PosY;		// 描画座標Y
	float m_Radius;		// 半径(描画用)
	float m_CenterX;	// 中心座標X
	float m_CenterY;	// 中心座標Y
	float m_Angle = 0;		// 角度
	float m_Length = 50;		// 半径の長さ

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