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
	void Update();
	void Draw();
	bool Initialize(XMFLOAT3 target = { 0,0,0 },bool flag = false);

private:

private:
	//Model* model = nullptr;
	OBJobject* object = nullptr;

	//’…’e–Ú•W
	XMFLOAT3 target;
	//”­ŽË‚³‚ê‚Ä‚¢‚é‚©ƒtƒ‰ƒO
	bool flag = false;
};

