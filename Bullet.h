#pragma once
#include <DirectXMath.h>
#include "Vector3.h"




class Bullet
{
private:
	Vector3 pos = { 0,0,0 };
	Vector3 vpos = { 1,1,1 };

	bool attackFlag = false;

public:
	void Update()
	{
		if (attackFlag)
		{
			Move();
		}

		//�����蔻��̏���


	}

	void SetAttack()
	{
		attackFlag = true;
	}

private:
	void Move()
	{
		pos = vpos;
	}
};

