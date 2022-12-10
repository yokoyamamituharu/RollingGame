#pragma once
#include <DirectXMath.h>

// DirectX::���ȗ�
using XMFLOAT2 = DirectX::XMFLOAT2;
using XMFLOAT3 = DirectX::XMFLOAT3;
using XMFLOAT4 = DirectX::XMFLOAT4;
using XMMATRIX = DirectX::XMMATRIX;
using XMVECTOR = DirectX::XMVECTOR;


//XMFLOAT3�̃I�[�o�[���[�h
const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs);

const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs);

const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs);

const DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& lhs, const DirectX::XMFLOAT3& rhs);

const DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& lhs, const float& scalar);

const DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& lhs, const float& scalar);

const DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& lhs, const DirectX::XMVECTOR& rhs);

const DirectX::XMFLOAT3 operator+(const DirectX::XMVECTOR& lhs, const DirectX::XMFLOAT3& rhs);

const DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& lhs, const DirectX::XMVECTOR& rhs);

const DirectX::XMFLOAT3 operator-(const DirectX::XMVECTOR& lhs, const DirectX::XMFLOAT3& rhs);

namespace Use
{
	XMFLOAT3 LoadXMVECTOR(const XMVECTOR& v);

}