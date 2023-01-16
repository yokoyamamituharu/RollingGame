#include "Useful.h"

const DirectX::XMFLOAT2 operator+(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs)
{
	XMFLOAT2 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	return result;
}

const DirectX::XMFLOAT2 operator-(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs)
{
	XMFLOAT2 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	return result;
}

const DirectX::XMFLOAT2 operator*(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs)
{
	XMFLOAT2 result;
	result.x = lhs.x * rhs.x;
	result.y = lhs.y * rhs.y;
	return result;
}

const DirectX::XMFLOAT2 operator/(const DirectX::XMFLOAT2& lhs, const DirectX::XMFLOAT2& rhs)
{
	XMFLOAT2 result;
	result.x = lhs.x / rhs.x;
	result.y = lhs.y / rhs.y;
	return result;
}

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const float& scalar)
{
	XMFLOAT3 result;
	result.x = lhs.x + scalar;
	result.y = lhs.y + scalar;
	result.z = lhs.z + scalar;
	return result;
}

const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& lhs, const float& scalar)
{
	XMFLOAT3 result;
	result.x = lhs.x - scalar;
	result.y = lhs.y - scalar;
	result.z = lhs.z - scalar;
	return result;
}


const DirectX::XMFLOAT2 operator*(const DirectX::XMFLOAT2& lhs, const float& scalar)
{
	XMFLOAT2 result;
	result.x = lhs.x * scalar;
	result.y = lhs.y * scalar;
	return result;
}

const DirectX::XMFLOAT2 operator/(const DirectX::XMFLOAT2& lhs, const float& scalar)
{
	XMFLOAT2 result;
	result.x = lhs.x / scalar;
	result.y = lhs.y / scalar;
	return result;
}

//XMFLOAT3ìØémÇÃâ¡éZèàóù
const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x + rhs.x;
	result.y = lhs.y + rhs.y;
	result.z = lhs.z + rhs.z;
	return result;
}

const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x - rhs.x;
	result.y = lhs.y - rhs.y;
	result.z = lhs.z - rhs.z;
	return result;
}

const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x * rhs.x;
	result.y = lhs.y * rhs.y;
	result.z = lhs.z * rhs.z;
	return result;
}

const DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x / rhs.x;
	result.y = lhs.y / rhs.y;
	result.z = lhs.z / rhs.z;
	return result;
}


const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& lhs, const float& scalar)
{
	XMFLOAT3 result;
	result.x = lhs.x * scalar;
	result.y = lhs.y * scalar;
	result.z = lhs.z * scalar;
	return result;
}

const DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& lhs, const float& scalar)
{
	XMFLOAT3 result;
	result.x = lhs.x / scalar;
	result.y = lhs.y / scalar;
	result.z = lhs.z / scalar;
	return result;
}

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMVECTOR& rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x + rhs.m128_f32[0];
	result.y = lhs.y + rhs.m128_f32[1];
	result.z = lhs.z + rhs.m128_f32[2];
	return result;
}

const DirectX::XMFLOAT3 operator+(const DirectX::XMVECTOR& lhs, const DirectX::XMFLOAT3& rhs)
{
	XMFLOAT3 result;
	result.x = lhs.m128_f32[0] + rhs.x;
	result.y = lhs.m128_f32[1] + rhs.y;
	result.z = lhs.m128_f32[2] + rhs.z;
	return result;
}

const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& lhs, const DirectX::XMVECTOR& rhs)
{
	XMFLOAT3 result;
	result.x = lhs.x - rhs.m128_f32[0];
	result.y = lhs.y - rhs.m128_f32[1];
	result.z = lhs.z - rhs.m128_f32[2];
	return result;
}

const DirectX::XMFLOAT3 operator-(const DirectX::XMVECTOR& lhs, const DirectX::XMFLOAT3& rhs)
{
	XMFLOAT3 result;
	result.x = lhs.m128_f32[0] - rhs.x;
	result.y = lhs.m128_f32[1] - rhs.y;
	result.z = lhs.m128_f32[2] - rhs.z;
	return result;
}


XMFLOAT3 Use::LoadXMVECTOR(const XMVECTOR& v)
{
	XMFLOAT3 output;
	output.x = v.m128_f32[0];
	output.y = v.m128_f32[1];
	output.z = v.m128_f32[2];
	return output;
}
