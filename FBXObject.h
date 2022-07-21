#pragma once
#include "Model.h"
#include "Camera.h"

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXMath.h>
#include <string>

class FBXObject
{
protected://�G�C���A�X
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://�T�u�N���X
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
	};

public://�ÓI�����o�֐�
	static void SetDevice(ID3D12Device* device) { FBXObject::device = device; }
	static void SetCamera(Camera* camera) { FBXObject::camera = camera; }

public:
	void Initialize();

private:
	static ID3D12Device* device;
	static Camera* camera;

protected:
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransform;
};

