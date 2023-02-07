#pragma once
#include "EnemyZako.h"

class YowaiEnemy :
	public EnemyZako
{
	private:
	//DirectXÇè»ó™
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	YowaiEnemy();
	~YowaiEnemy();

	void InitializeOut(XMFLOAT3 pos, bool isTarget, XMFLOAT3 targetPos1 = { 0,0,0 }, XMFLOAT3 targetPos2 = { 0,0,0 })override;

	void InitializeIn()override;
};

