#include "Enemy.h"

float Enemy::CalcGravity()
{
	const float gravity = 9.8f;
	return 2 / 1 * gravity * airTime * airTime;
}