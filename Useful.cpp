#include "Useful.h"

XMFLOAT3 Use::LoadXMVECTOR(const XMVECTOR& v)
{
	XMFLOAT3 output;
	output.x = v.m128_f32[0];
	output.y = v.m128_f32[1];
	output.z = v.m128_f32[2];
	return output;
}
