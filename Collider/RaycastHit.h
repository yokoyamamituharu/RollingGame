#pragma once

#include "BaseCollider.h"
#include <DirectXMath.h>

class ObjectObj;

/// <summary>
/// レイキャストによる情報を得る為の構造体
/// </summary>
struct RaycastHit
{
	// 衝突相手のオブジェクト
	ObjectObj* object = nullptr;
	// 衝突相手のコライダー
	BaseCollider* collider = nullptr;
	// 衝突点
	DirectX::XMVECTOR inter;
	// 衝突点までの距離
	float distance = 0.0f;
};

