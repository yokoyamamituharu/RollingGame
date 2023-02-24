#pragma once
#include "ObjectObj.h"
#include "Input.h"
#include "InputMouse.h"
#include <DirectXMath.h>
#include "Player.h"
#include "BaseEnemy.h"

class EnemyZako :
	public BaseEnemy
{
private:
	//DirectXを省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:	//メンバ関数	

	//コンストラクタ
	EnemyZako();

	//デストラクタ
	~EnemyZako();

	virtual void InitializeOut(XMFLOAT3 pos, bool isTarget, XMFLOAT3 targetPos1 = { 0,0,0 }, XMFLOAT3 targetPos2 = { 0,0,0 });

	virtual void InitializeIn();

};