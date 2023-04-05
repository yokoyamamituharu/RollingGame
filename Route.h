#pragma once
#include <DirectXMath.h>

class Route
{
private:
	// DirectX::Çè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:
	static Route* GetIns();

	static void Set();

	static XMFLOAT2* GetRoute(const int index);

public:
	static XMFLOAT2 route[5][3];

};

