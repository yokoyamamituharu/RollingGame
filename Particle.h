#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>
#include "ObjectObj.h"
#include "ParticleTube.h"


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
	

public:

	void Add(std::unique_ptr<ParticleTubu> tubu);

	void Initialize();

	void Update();

	void Draw();

private: // メンバ変数

public:
	//パーティクル配列
	std::list<std::unique_ptr<ParticleTubu>> particles;

};

