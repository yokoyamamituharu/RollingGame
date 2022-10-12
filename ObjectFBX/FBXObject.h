#pragma once
#include "FbxModel.h"
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

public://�萔
	//�{�[���̍ő吔
	static const int MAX_BONES = 32;

public://�T�u�N���X
	struct ConstBufferDataTransform
	{
		XMMATRIX viewproj;
		XMMATRIX world;
		XMFLOAT3 cameraPos;
	};

	//�萔�o�b�t�@�p�f�[�^�\����(�X�L�j���O)
	struct ConstBufferDataSkin
	{
		XMMATRIX bones[MAX_BONES];
	};



public://�ÓI�����o�֐�
	static bool StaticInitialize(ID3D12Device* device, Camera* camera);

	static void SetDevice(ID3D12Device* device) { FBXObject::device = device; }
	static void SetCamera(Camera* camera) { FBXObject::camera = camera; }
	//�O���b�t�B�N�p�C�v���C���̐���
	static void CreateGraphicsPipline();

public:
	//�R���X�g���N�^
	FBXObject();
	//�f�X�g���N�^
	~FBXObject();

	//�A�j���[�V�����̊J�n
	void PlayAnimetion();

	void Initialize();
	//���t���[������
	void Update();
	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

	void SetModel(FbxModel* model) { this->model = model; }

public://�Q�b�^�[�E�Z�b�^�[
	void SetPos(XMFLOAT3 pos) { position = pos; }

private:
	static ID3D12Device* device;
	static Camera* camera;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature>rootsignature;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState>pipelinestate;

protected:
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffTransform;
	//���[�J���X�P�[��
	XMFLOAT3 scale = {1,1,1 };
	//X,Y,Z�����̃��[�J����]�p
	XMFLOAT3 rotation = { 0,0,0 };
	//���[�J�����W
	XMFLOAT3 position = { 0,0,0 };
	//���[�J�����[���h�ϊ��s��
	XMMATRIX matWorld;
	//���f��
	FbxModel* model = nullptr;
	//�萔�o�b�t�@�X�L��
	ComPtr<ID3D12Resource>constBuffSkin;

	//1�t���[���̎���
	FbxTime frameTime;
	//�A�j���[�V�����J�n����
	FbxTime startTime;
	//�A�j���[�V�����I������
	FbxTime endTime;
	//���ݎ��ԁi�A�j���[�V�����j
	FbxTime currentTime;
	//�A�j���[�V�����Đ���
	bool isPlay = false;

};

