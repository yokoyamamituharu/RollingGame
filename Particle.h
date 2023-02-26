#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>
#include <forward_list>
#include "ObjectObj.h"

/// <summary>
/// 3D�I�u�W�F�N�g
/// </summary>
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
	//�p�[�e�B�N��1��
	struct ParticleTubu
	{
		//DirectX::���ȗ�
		using XMFLOAT3 = DirectX::XMFLOAT3;

		//���W
		XMFLOAT3 position = {};
		//���x
		XMFLOAT3 velocity = {};
		//�X�P�[���̕ω�
		float startScale = 0.0f;
		float endScale = 0.0f;
		//���݂̃t���[��
		int frame = 0;
		//�I���t���[��
		int end_frame = 0;

		//�X�P�[��
		float scale = 1.0f;
		//�����l
		//float s_scale = 1.0f;
		//�ŏI�l
		//float e_scale = 1.0f;
		ObjectObj* obj = nullptr;
	};

public:
	static Particle* GetIns();

	void Add(ParticleTubu* tubu);

	void Initialize();

	void Update();

	void Draw();

private: // �����o�ϐ�



public:
	//�p�[�e�B�N���z��
	std::forward_list<ParticleTubu> particles;

};

