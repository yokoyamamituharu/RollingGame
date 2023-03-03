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
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	

public:

	void Add(std::unique_ptr<ParticleTubu> tubu);

	void Initialize();

	void Update();

	void Draw();

private: // �����o�ϐ�

public:
	//�p�[�e�B�N���z��
	std::list<std::unique_ptr<ParticleTubu>> particles;

};

