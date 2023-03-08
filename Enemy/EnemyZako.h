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

	virtual void InitializeOut(bool isTarget,XMFLOAT2 route[]);

	virtual void InitializeIn();

};