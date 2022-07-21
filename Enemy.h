#pragma once
#include<DirectXMath.h>

class Enemy
{
	float CalcGravity();

private:
	unsigned hp = 10;
	unsigned status = 0;

	float airTime = 0;

	DirectX::XMFLOAT3 pos = { 0,0,0 };
};

