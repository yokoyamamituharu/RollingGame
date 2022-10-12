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

private:

private:
	//Model* model = nullptr;
	OBJobject* object = nullptr;

	//���e�ڕW
	XMFLOAT3 target;
	//���˂���Ă��邩�t���O
	bool flag = false;
	bool dead = false;
	int timer = 0;
};
