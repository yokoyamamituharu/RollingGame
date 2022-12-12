﻿#pragma once

#include <DirectXMath.h>

class ObjectObj;
class BaseCollider;

/// <summary>
/// 衝突情報
/// </summary>
struct CollisionInfo
{
public:
	CollisionInfo(ObjectObj* object, BaseCollider* collider, const DirectX::XMVECTOR& inter) {
		this->object = object;
		this->collider = collider;
		this->inter = inter;
	}

	// 衝突相手のオブジェクト
	ObjectObj* object = nullptr;
	// 衝突相手のコライダー
	BaseCollider* collider = nullptr;
	// 衝突点
	DirectX::XMVECTOR inter;
};

