#pragma once
#include "ObjectObj.h"


//パーティクル1粒
class ParticleTubu
{
public:
	ParticleTubu();
	~ParticleTubu();

public:
	//DirectX::を省略
	using XMFLOAT3 = DirectX::XMFLOAT3;

	//座標
	XMFLOAT3 position = {};
	//速度
	XMFLOAT3 velocity = {};
	//スケールの変化
	float startScale = 0.0f;
	float endScale = 0.0f;
	//現在のフレーム
	int frame = 0;
	//終了フレーム
	int end_frame = 0;

	//スケール
	float scale = 1.0f;
	//初期値
	//float s_scale = 1.0f;
	//最終値
	//float e_scale = 1.0f;

public:
	std::unique_ptr<ObjectObj> obj = nullptr;
};
