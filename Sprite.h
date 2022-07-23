#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class Sprite
{
protected://�G�G�C���A�X
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public://�T�u�N���X

	//�V�������_�\���̂̒�`
	struct VertexPosUv
	{
		XMFLOAT3 pos;
		XMFLOAT2 uv;
	};


	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData {
		XMFLOAT4 color;		//�F(RGBA)
		XMMATRIX mat;		//3D�ϊ��s��
	};


protected://���I�����o�ϐ�
	//�e�N�X�`���̍ő喇��
	static const int srvCount = 512;
	//���_��
	static const int vertNum = 4;
	//�f�o�C�X
	static ID3D12Device* device;
	//�f�X�N���v�^�T�C�Y
	static UINT descriptorHandleIncrementSize;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//���[�g�V�O�l�`��
	static ComPtr<ID3D12RootSignature>rootSignature;
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	static ComPtr<ID3D12PipelineState>pipelineState;
	//�ˉe�s��
	static XMMATRIX matProjection;
	//�f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap>descHeap;
	//�e�N�X�`���o�b�t�@
	static ComPtr<ID3D12Resource>texBuffer[srvCount];




protected://�����o�ϐ�
	ComPtr<ID3D12Resource>vertBuff;		//���_�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vbView{};	//���_�o�b�t�@�r���[
	ComPtr<ID3D12Resource> constBuff;//�萔�o�b�t�@
	//Z�����̉�]�p
	float rotation = 0.0f;
	//���W
	XMFLOAT2 position = { 0,0 };
	//�傫��
	XMFLOAT2 size = { 100,100 };
	//���[���h�s��
	XMMATRIX matWorld;
	//�F(RGBA)
	XMFLOAT4 color = { 1,1,1,1 };
	//�e�N�X�`���ԍ�
	UINT texNumber = 0;

public://�ÓI�����o�֐�
	//�ÓI������
	static bool StaticInitialize(ID3D12Device* device, int window_width, int window_height);

	//�e�N�X�`���ǂݍ���
	static bool LoadTexture(UINT texnumber, const wchar_t* filename);

	//�`��O����
	static void PreDraw(ID3D12GraphicsCommandList* cmdList);

	//�`��㏈��
	static void PostDraw();

	//�������ꂽ�X�v���C�g
	static Sprite* Create(UINT texNumber, XMFLOAT2 position, XMFLOAT4 color = { 1, 1, 1, 1 });

public://�����o�֐�

	//�R���X�g���N�^	
	Sprite(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color);

	//������	
	bool Initialize();

	//�X�v���C�g�P�̕`��
	void Draw();

protected:
	//���_�f�[�^�]��
	void TransferVertices();
};