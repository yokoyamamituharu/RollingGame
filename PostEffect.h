#pragma once
#include "Sprite.h"
class PostEffect :
	public Sprite
{
public:
	PostEffect();

	//������
	void Initialize();

	//�V�[���`��O����
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	//�V�[���`��㏈��
	void PosDrawScene(ID3D12GraphicsCommandList* cmdList);

	void Draw(ID3D12GraphicsCommandList* cmdList);

	int GetPipelineNum() { return pipelineNum; }

private:
	//�p�C�v���C������
	void CreateGraphicsPipeLineState();


private://�ÓI�����o�ϐ�
	//��ʃN���A�J���[
	static const float clearColor[4];


private://�����o�ϐ�
	//�|�X�g�G�t�F�N�g�̐�
	static const int postNum = 3;
	ComPtr<ID3D12Resource>texBuff[2];
	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap>descHeapSRV;
	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource>depthBuff;
	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap>descHeapRTV;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap>descHeapDSV;

	//�O���t�B�b�N�X�p�C�v���C��
	ComPtr<ID3D12PipelineState> pipelineState[postNum];
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;

	//�O���t�B�b�N�p�C���v���C���I��p�ϐ�
	int pipelineNum = 0;

};

