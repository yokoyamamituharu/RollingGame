#pragma once
#include "ObjectOBJ.h"
#include "Input.h"
#include "InputMouse.h"
#include <DirectXMath.h>

class Player
{
private:
	//DirectXを省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;


public:
	Player();
	~Player();

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

	void Res(bool flag = false, XMFLOAT3 vec = { 0,0,0 });

	void Stop();

public:
	void SetInput(Input* input) { this->input = input; }
	void SetMouse(InputMouse* mouse) { this->mouse = mouse; }
	void SetCamera(Camera* camera) { this->camera = camera; }

	float Ease(float x, float s);

	int GetHp() { return hp; }
	int GetMaxHp() { return maxHp; }
	void Damage(int damage);
	void Cure(int amount) { hp += amount; }

public:
	ObjectObj* object = nullptr;

	bool attackFlag = false;

	float grundHeight = -6.0f;

	static int breakEnemy;

	XMFLOAT3 outPos = { 0,0,0 };	//中シーンに行くときに今の座標を保存しておく
private:
	Input* input;
	InputMouse* mouse;
	Model* playermodel = nullptr;
	Model* playerSpheremodel = nullptr;
	bool muteki = false;
	int mutekiTime = 0;

	bool isSphere = false;		//クリックしてるとき
	bool sphereFlag = false;	//玉の状態のとき
	DirectX::XMFLOAT3 spiralSpeed = {};
	float rollingSpeed = 0.0f;

	Camera* camera;
	bool isCamera = false;

	int sceneType = 1;

	int rollingTime = 0;

	bool resFlag1 = false;
	bool resFlag2 = false;
	bool resFlag3 = false;
	int resTimer = 0;

	float riseValue = 20;
	float posY = 0;
	float timer = 0;

	float backValue = 15;

	float gravity = 0.0f;

	int maxHp = 5;
	int hp = maxHp;

	XMFLOAT3 backVec = { 0,0,0 };

	XMFLOAT3 move = { 0,0,0 };

};

