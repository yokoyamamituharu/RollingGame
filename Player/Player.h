#pragma once
#include "ObjectObj.h"
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

	static Player* Create(Camera* camera, int InOrOut);
	void Initialize(Camera* camera, int InOrOut);
	void UpdateOut(Camera* camera);
	void UpdateIn();
	void Move();
	void MoveIn();
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

	void Draw();

	bool GetRes();

	void HitCrowAttack(XMFLOAT3 pos = { 0,-100,0 });


	void CrowAttack();

	bool GetCrow() { return crawAttackFlag; }


	bool GetRolling() { return rollingSpeed > 0; }

	bool GetSphere() { return sphereFlag; }
public:
	void SetCamera(Camera* camera) { this->camera = camera; }

	float Ease(float x, float s);

	int GetHp() { return hp; }
	int GetMaxHp() { return maxHp; }
	void Damage(int damage);
	void Cure(int amount) { hp += amount; }
	void SetHp(int hp) { this->hp = hp; }
	void StopRolling();

public:
	ObjectObj* object = nullptr;

	bool attackFlag = false;

	const static float grundHeight;

	static int breakEnemy;

	//丸影
	ObjectObj* shadowObj = nullptr;

	XMFLOAT3 outPos = { 0,0,0 };	//中シーンに行くときに今の座標を保存しておく
private:
	Model* playermodel = nullptr;
	Model* playerSpheremodel = nullptr;
	bool muteki = false;
	int mutekiTime = 0;

	bool isSphere = false;		//クリックしてるとき
	bool sphereFlag = false;	//玉の状態のとき
	DirectX::XMFLOAT3 spiralSpeed = {};
	float rollingSpeed = 0.0f;

	Camera* camera;

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

	static int maxHp;
	static int hp;

	XMFLOAT3 backVec = { 0,0,0 };

	XMFLOAT2 clickTrigerPos = { 0,0 };
	XMVECTOR attackDirection;

	bool crawAttackFlag = 0;
	XMFLOAT3 crawTargetPos = { 0,0,0 };
	XMFLOAT3 crawDirection = { 0,0,0 };
private:
	//接地フラグ
	bool onGround = true;
	//落下ベクトル
	XMVECTOR fallV;
	ObjectObj* waveright[4];
	ObjectObj* waveleft[4];

	XMFLOAT3 attackRota = { 0,0,0 };

	int waveTime = 0;
	int waveIndex = 0;

	bool goFlag = false;

	int waveNum = 0;
public:
	XMFLOAT3 move = { 0,0,0 };
};

