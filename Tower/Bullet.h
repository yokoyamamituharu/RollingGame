#pragma once
#include "ObjectObj.h"	
class Bullet
{
	using XMFLOAT3 = DirectX::XMFLOAT3;
public:
	static Bullet* Create();
public:
	Bullet();
	~Bullet();
	bool Initialize(XMFLOAT3 pos = { 0,0,0 }, XMFLOAT3 target = { 0,0,0 }, bool flag = false);
	void Update();
	void Draw();
	bool GetDead() { return dead; }
	void Dead() { dead = true; }

public:
	ObjectObj* object = nullptr;
private:
	//着弾目標
	XMFLOAT3 target;
	//発射されているかフラグ
	bool flag = false;
	bool dead = false;
	const int aliveTime = 60;
	int timer = aliveTime;
};

