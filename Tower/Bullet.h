#pragma once
#include "OBJObject.h"	
class Bullet
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
public:
	static Bullet* Create();
	static void StaticInitialize();
	static Model* model;
public:
	Bullet();
	~Bullet();
	bool Initialize(XMFLOAT3 pos = { 0,0,0 },XMFLOAT3 target = { 0,0,0 }, bool flag = false);
	void Update();
	void Draw();
	bool IsDead() { return dead; }

public:
OBJobject* object = nullptr;
private:
	//Model* model = nullptr;
	

	//着弾目標
	XMFLOAT3 target;
	//発射されているかフラグ
	bool flag = false;
	bool dead = false;
	int timer = 0;
};

