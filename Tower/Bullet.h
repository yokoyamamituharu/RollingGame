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
	//’…’e–Ú•W
	XMFLOAT3 target;
	//”­ŽË‚³‚ê‚Ä‚¢‚é‚©ƒtƒ‰ƒO
	bool flag = false;
	bool dead = false;
	int timer = 0;
};

