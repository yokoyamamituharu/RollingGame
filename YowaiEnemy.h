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

	void CreateEnemy()override;
	//void InitIndividualSetUp()override;

	static std::shared_ptr<YowaiEnemy> Create(bool isTarget, XMFLOAT2 route[] = nullptr);


	void InitializeIn()override;
};

