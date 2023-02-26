#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>
#include "ObjectObj.h"

/// <summary>
/// 3Dオブジェクト
/// </summary>
class Particle
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
	//パーティクル1粒
	struct ParticleTubu
	{
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
		ObjectObj* obj = nullptr;
	};

public:
	static Particle* GetIns();

	void Add(ParticleTubu* tubu);

	void Initialize();

	void Update();

	void Draw();

private: // メンバ変数



public:
	//パーティクル配列
	std::forward_list<ParticleTubu> particles;

};

