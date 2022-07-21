#pragma region include�֘A��WindowProc

#include<DirectXTex.h>

#include<d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

#include <d3d12.h>
#include <dxgi1_6.h>

#include<vector>
#include<string>

#include<DirectXMath.h>
using namespace DirectX;

#pragma comment(lib, "d3d12.lib")

#pragma comment(lib, "dxgi.lib")

//����
#define DIRECTINPUT_VERSION 0x0800		//DirectInput�̃o�[�W�����w��
#include <dinput.h>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")



#pragma endregion
#include<d3dx12.h>
#include<stdlib.h>
#include<time.h>

#include"Input.h"
#include "WinApp.h"
#include"DirectXCommon.h"
#include"OBJobject.h"

using namespace Microsoft::WRL;

#include"fbxsdk.h"
#include"FbxLoader.h"
#include "Camera.h"
#include "FBXObject.h"

//�����̂̓����蔻��
bool CubeCollision(XMFLOAT3 object1, XMFLOAT3 radius1, XMFLOAT3 object2, XMFLOAT3 radius2) {
	float disX1 = object1.x + radius1.x;
	float disX2 = object2.x - radius2.x;
	float disX3 = object1.x - radius1.x;
	float disX4 = object2.x + radius2.x;
	float disY1 = object1.y + radius1.y;
	float disY2 = object2.y - radius2.y;
	float disY3 = object1.y - radius1.y;
	float disY4 = object2.y + radius2.y;
	float disZ1 = object1.z + radius1.z;
	float disZ2 = object2.z - radius2.z;
	float disZ3 = object1.z - radius1.z;
	float disZ4 = object2.z + radius2.z;

	//x��y�͂��ꂼ�ꒆ�S���W�Ƃ��Čv�Z����
	return disX1 > disX2 && disX4 > disX3 && disY1 > disY2 && disY4 > disY3 && disZ1 > disZ2 && disZ4 > disZ3;
}

#pragma region �X�v���C�g
//�e�N�X�`���̍ő喇��
const int spriteSRVCount = 512;

//�p�C�v���C���Z�b�g
struct PipelineSet
{
	//�p�C�v���C���X�e�[�g�I�u�W�F�N�g
	ComPtr<ID3D12PipelineState>pipelinestate;
	//���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature>rootsignature;
};

//�萔�o�b�t�@�p�f�[�^�\����
struct ConstBufferData {
	XMFLOAT4 color;		//�F(RGBA)
	XMMATRIX mat;		//3D�ϊ��s��
};

struct Vertex
{
	XMFLOAT3 pos;	//xyz���W
	XMFLOAT3 normal;//�@���x�N�g��
	XMFLOAT2 uv;	//u��
};

//�V�������_�\���̂̒�`
struct VertexPosUv
{
	XMFLOAT3 pos;
	XMFLOAT2 uv;
};

//�X�v���C�g1�����̃f�[�^
struct Sprite
{
	ComPtr<ID3D12Resource>vertBuff;		//���_�o�b�t�@
	D3D12_VERTEX_BUFFER_VIEW vbView{};	//���_�o�b�t�@�r���[
	ComPtr<ID3D12Resource> constBuff = nullptr;//�萔�o�b�t�@
	//Z�����̉�]�p
	float rotation = 0.0f;
	//���W
	XMFLOAT3 position = { 0,0,0 };
	//���[���h�s��
	XMMATRIX matWorld;
	//�F(RGBA)
	XMFLOAT4 color = { 1,1,1,1 };
	//�e�N�X�`���ԍ�
	UINT texNumber = 0;
	//�傫��
	XMFLOAT2 size = { 100,100 };
};

//�X�v���C�g�̋��ʃf�[�^
struct SpriteCommon
{
	//�p�C�v���C���Z�b�g
	PipelineSet pipelineSet;
	//�ˉe�s��
	XMMATRIX matProjection{};
	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐���
	ComPtr<ID3D12DescriptorHeap> descHeap;
	//�e�N�X�`�����\�[�X�i�e�N�X�`���o�b�t�@�j�̔z��
	ComPtr<ID3D12Resource>texBuffer[spriteSRVCount];
};

//�X�v���C�g�P�̒��_�o�b�t�@�̓]��
void SpriteTransferVertexBuffer(const Sprite &sprite)
{
	HRESULT result = S_FALSE;

	//���_�f�[�^
	VertexPosUv vertices[] = {
		{{},{0.0f,1.0f}},//����
		{{},{0.0f,0.0f}},//����
		{{},{1.0f,1.0f}},//�E��
		{{},{1.0f,0.0f}},//�E��
	};

	//�����A����A�E���A�E��
	enum { LB, LT, RB, RT };

	vertices[LB].pos = { 0.0f,sprite.size.y,0.0f };//����
	vertices[LT].pos = { 0.0f,0.0f,0.0f };//����
	vertices[RB].pos = { sprite.size.x,sprite.size.y,0.0f };//�E��
	vertices[RT].pos = { sprite.size.x,0.0f,0.0f };//�E��

	//���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv *vertMap = nullptr;
	result = sprite.vertBuff->Map(0, nullptr, (void **)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.vertBuff->Unmap(0, nullptr);
}

//�X�v���C�g�p�p�C�v���C�������֐�
PipelineSet SpriteCreateGraphicsPipeline(ID3D12Device *dev)
{
	//���[�J����result�𐶐�����
	HRESULT result;


	ComPtr<ID3DBlob> vsBlob = nullptr;//���_�V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob = nullptr;//�s�N�Z���V�F�[�_�[�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob = nullptr;//�G���[�I�u�W�F�N�g

	//���_�V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"SpriteVS.hlsl",//�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h�\�ɂ���
		"main", "vs_5_0",//�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�Ńo�b�N�p�ݒ�
		0,
		&vsBlob, &errorBlob);


	//�s�N�Z���V�F�[�_�[�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"SpritePS.hlsl",//�V�F�[�_�[�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,//�C���N���[�h���\�ɂ���
		"main", "ps_5_0",//�G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,//�f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);


	//���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{//xyz���W
			"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		},
		{//uv���W
			"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
		}
	};

	//�O���t�B�b�N�X�p�C�v���C���̐ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};

	//���_�V�F�[�_�[�A�s�N�Z���V�F�[�_�[���p�C�v���C���ɐݒ�
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;//�W���ݒ�

	//���X�^���C�U�X�e�[�g
	//�W���I�Ȑݒ�(�w�ʃJ�����O�A�h��Ԃ��A�[�x�N���b�s���O�L��)
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);	//��U�W���l���Z�b�g
	gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;			//�w�ʃJ�����O�����Ȃ�



	//�u�����h�X�e�[�g�̐ݒ�
	//�����_�[�^�[�Q�b�g�̃u�����h�ݒ�
	D3D12_RENDER_TARGET_BLEND_DESC &blenddesc = gpipeline.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;


	//�f�v�X�X�e���V���X�e�[�g�̐ݒ�
//�W���I�Ȑݒ�i�[�x�e�X�g���s���A�������݋��A�[�x����������΍��i�j
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);	//��U�W���l���Z�b�g
	gpipeline.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_ALWAYS;		//��ɏ㏑�����[��
	gpipeline.DepthStencilState.DepthEnable = false;							//�[�x�e�X�g�����Ȃ�

	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g

		//���_���C�A�E�g�̐ݒ�
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	//�}�`�̌`����O�p�`�ɐݒ�
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	//���̑��̐ݒ�
	gpipeline.NumRenderTargets = 1;//�`��Ώۂ�1��
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;//0�`255�w���RGBA
	gpipeline.SampleDesc.Count = 1;//1�s�N�Z���ɂ�1��T���v�����O

	//�X�^�e�B�b�N�T���v���[(�e�N�X�`���T���v���[)
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);


	CD3DX12_DESCRIPTOR_RANGE  descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);//t0���W�X�^



	CD3DX12_ROOT_PARAMETER rootparams[2];
	rootparams[0].InitAsConstantBufferView(0);
	rootparams[1].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc,
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);


	//�p�C�v�����ƃ��[�g�V�O�l�`���̃Z�b�g
	PipelineSet pipelineSet;


	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	//�o�[�W������������ł̃V���A���C�Y
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);

	//���[�g�V�O�l�`���̐���
	result = dev->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&pipelineSet.rootsignature));

	//�p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	gpipeline.pRootSignature = pipelineSet.rootsignature.Get();


	result = dev->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelineSet.pipelinestate));



	//�p�C�v���C���ƃ��[�g�V�O�l�`����Ԃ�
	return pipelineSet;
}

//�X�v���C�g���ʃf�[�^����
SpriteCommon SpriteCommonCreate(ID3D12Device *dev, int window_width, int window_height)
{
	HRESULT result = S_FALSE;

	//�V���ȃX�v���C�g���ʃf�[�^�𐶐�
	SpriteCommon spriteCommon{};

	//�f�X�N���v�^�q�[�v�𐶐�
	D3D12_DESCRIPTOR_HEAP_DESC descHeapDesc = {};
	descHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	descHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	descHeapDesc.NumDescriptors = spriteSRVCount;
	result = dev->CreateDescriptorHeap(&descHeapDesc, IID_PPV_ARGS(&spriteCommon.descHeap));

	//�X�v���C�g�p�p�C�v���C������
	spriteCommon.pipelineSet = SpriteCreateGraphicsPipeline(dev);

	//���s���e�ˉe�s�񐶐�
	spriteCommon.matProjection = XMMatrixOrthographicOffCenterLH(
		0.0f, (float)window_width, (float)window_height, 0.0f, 0.0f, 1.0f);

	//���������X�v���C�g���ʃf�[�^��Ԃ�
	return spriteCommon;
}

//�X�v���C�g����
Sprite SpriteCreate(ID3D12Device *dev, int window_width, int window_height, UINT texNumber, const SpriteCommon &spriteCommon)
{
	HRESULT result = S_FALSE;

	//�V�����X�v���C�g�����
	Sprite sprite{};

	//�e�N�X�`���ԍ����R�s�[
	sprite.texNumber = texNumber;

	//���_�f�[�^
	VertexPosUv vertices[] =
	{
		{{  0.0f,100.0f,0.0f},{0.0f,1.0f}},
		{{  0.0f,  0.0f,0.0f},{0.0f,0.0f}},
		{{100.0f,100.0f,0.0f},{1.0f,1.0f}},
		{{100.0f,  0.0f,0.0f},{1.0f,0.0f}}
	};

	//���_�o�b�t�@����
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&sprite.vertBuff));

	//�w��ԍ��̉摜���ǂݍ��ݍς݂Ȃ�
	if (spriteCommon.texBuffer[sprite.texNumber])
	{
		//�e�N�X�`�����擾
		D3D12_RESOURCE_DESC resDesc = spriteCommon.texBuffer[sprite.texNumber]->GetDesc();
		//�X�v���C�g�̑傫�����摜�̉𑜓x�ɍ��킹��
		sprite.size = { (float)resDesc.Width,(float)resDesc.Height };
	}
	SpriteTransferVertexBuffer(sprite);

	//���_�o�b�t�@�ւ̃f�[�^�]��
	VertexPosUv *vertMap = nullptr;
	result = sprite.vertBuff->Map(0, nullptr, (void **)&vertMap);
	memcpy(vertMap, vertices, sizeof(vertices));
	sprite.vertBuff->Unmap(0, nullptr);

	//���_�o�b�t�@�r���[�̍쐬
	sprite.vbView.BufferLocation = sprite.vertBuff->GetGPUVirtualAddress();
	sprite.vbView.SizeInBytes = sizeof(vertices);
	sprite.vbView.StrideInBytes = sizeof(vertices[0]);

	//�萔�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&sprite.constBuff));

	// �萔�o�b�t�@�Ƀf�[�^�]��
	ConstBufferData *constMap = nullptr;
	result = sprite.constBuff->Map(0, nullptr, (void **)&constMap);
	constMap->color = XMFLOAT4(1, 1, 1, 1); //�F�w��(RGBA)
	//���s���e�s��
	constMap->mat = XMMatrixOrthographicOffCenterLH(
		0.0f, window_width, window_height, 0.0f, 0.0f, 1.0f);
	sprite.constBuff->Unmap(0, nullptr);

	return sprite;
}

//�X�v���C�g���ʃe�N�X�`���ǂݍ���
void SpriteCommonLoadTexture(SpriteCommon &spriteCommon, UINT texnumber, const wchar_t *filename, ID3D12Device *dev)
{
	//�ُ�Ȕԍ��̎w������o
	assert(texnumber <= spriteSRVCount - 1);

	HRESULT result;

	//WIC�e�N�X�`���̃��[�h
	TexMetadata metadata{};
	ScratchImage scratchImg{};

	result = LoadFromWICFile(
		filename,
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	const Image *img = scratchImg.GetImage(0, 0, 0);	//���f�[�^�[���o


	//���\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		metadata.format,
		metadata.width,
		(UINT)metadata.height,
		(UINT16)metadata.arraySize,
		(UINT16)metadata.mipLevels);

	//�e�N�X�`���p�o�b�t�@�̐���
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK, D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,//�e�N�X�`���p�w��
		nullptr,
		IID_PPV_ARGS(&spriteCommon.texBuffer[texnumber]));

	// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	result = spriteCommon.texBuffer[texnumber]->WriteToSubresource(
		0,
		nullptr, // �S�̈�փR�s�[
		img->pixels,    // ���f�[�^�A�h���X
		(UINT)img->rowPitch,    // 1���C���T�C�Y
		(UINT)img->slicePitch   // 1���T�C�Y
	);

	//�V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};//�ݒ�\����
	//srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;//RGBA
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	//�q�[�v��texnumber�ԖڂɃV�F�[�_�[���\�[�X�r���[�쐬
	dev->CreateShaderResourceView(
		spriteCommon.texBuffer[texnumber].Get(),//�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,//�e�N�X�`���ݒ���
		CD3DX12_CPU_DESCRIPTOR_HANDLE(spriteCommon.descHeap->GetCPUDescriptorHandleForHeapStart(), texnumber,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);

	result = S_OK;
}

//�X�v���C�g�P�̍X�V
void SpriteUpdate(Sprite &sprite, const SpriteCommon &spriteCommon)
{
	//���[���h�s��̍X�V
	sprite.matWorld = XMMatrixIdentity();
	//Z����]
	sprite.matWorld *= XMMatrixRotationZ(XMConvertToRadians(sprite.rotation));
	//���s�ړ�
	sprite.matWorld *= XMMatrixTranslation(sprite.position.x, sprite.position.y, sprite.position.z);

	//�萔�o�b�t�@�̓]��
	ConstBufferData *constMap = nullptr;
	HRESULT result = sprite.constBuff->Map(0, nullptr, (void **)&constMap);
	constMap->mat = sprite.matWorld * spriteCommon.matProjection;
	constMap->color = sprite.color;
	sprite.constBuff->Unmap(0, nullptr);
}

//�X�v���C�g���ʃO���t�B�b�N�R�}���h�̃Z�b�g
void SpriteCommonBeginDraw(const SpriteCommon &spriteCommon, ID3D12GraphicsCommandList *cmdList)
{
	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(spriteCommon.pipelineSet.pipelinestate.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(spriteCommon.pipelineSet.rootsignature.Get());
	//�v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐ݒ�
	ID3D12DescriptorHeap *ppHeaps[] = { spriteCommon.descHeap.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

//�X�v���C�g�P�̕`��
void SpriteDraw(const Sprite &sprite, ID3D12GraphicsCommandList *cmdList, const SpriteCommon &spriteCommon,
	ID3D12Device *dev)
{
	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &sprite.vbView);

	//�萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, sprite.constBuff->GetGPUVirtualAddress());

	//�V�F�[�_���\�[�X�r���[���Z�b�g
	cmdList->SetGraphicsRootDescriptorTable(1,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			spriteCommon.descHeap->GetGPUDescriptorHandleForHeapStart(),
			sprite.texNumber,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));

	//�|���S���̕`��i4���_�Ŏl�p�`�j
	cmdList->DrawInstanced(4, 1, 0, 0);
}
#pragma endregion

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	HRESULT result;
	FbxManager* fbxManager = FbxManager::Create();
#pragma region ����������

	// DirectX�����������@��������
	//�|�C���^�u����
	WinApp *winApp = nullptr;
	winApp = new WinApp();
	winApp->Initialize();

	DirectXCommon *dxCommon = nullptr;
	//DirectX�̏�����
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	//����
	Input *input = nullptr;
	input = new Input();
	input->Initialize(winApp->GetHInstance(), winApp->GetHwnd());

	Camera* camera =nullptr;
	camera = new Camera();
	camera->StaticInitialize(WinApp::window_width, WinApp::window_height);

	//���f���̐ÓI������
	Model::StaticInitialize(dxCommon->GetDev());
	//3D�I�u�W�F�N�g�ÓI������
	OBJobject::StaticInitialize(dxCommon->GetDev(), WinApp::window_width, WinApp::window_height,camera);	

	//FBX
	FBXObject::SetDevice(dxCommon->GetDev());
	FBXObject::SetCamera(camera);

	PipelineSet spritePipelineSet = SpriteCreateGraphicsPipeline(dxCommon->GetDev());
	//�X�v���C�g���ʃf�[�^
	SpriteCommon spriteCommon;
	//�X�v���C�g���ʃf�[�^����
	spriteCommon = SpriteCommonCreate(dxCommon->GetDev(), winApp->window_width, winApp->window_height);
	//�X�v���C�g���ʃe�N�X�`���ǂݍ���
	SpriteCommonLoadTexture(spriteCommon, 0, L"Resources/texture.jpg", dxCommon->GetDev());
	SpriteCommonLoadTexture(spriteCommon, 1, L"Resources/sora.png", dxCommon->GetDev());
	SpriteCommonLoadTexture(spriteCommon, 2, L"Resources/make.png", dxCommon->GetDev());
	SpriteCommonLoadTexture(spriteCommon, 3, L"Resources/kati.png", dxCommon->GetDev());
	SpriteCommonLoadTexture(spriteCommon, 10, L"Resources/0.png", dxCommon->GetDev());
	SpriteCommonLoadTexture(spriteCommon, 11, L"Resources/1.png", dxCommon->GetDev());
	SpriteCommonLoadTexture(spriteCommon, 12, L"Resources/2.png", dxCommon->GetDev());
	SpriteCommonLoadTexture(spriteCommon, 13, L"Resources/3.png", dxCommon->GetDev());
	SpriteCommonLoadTexture(spriteCommon, 14, L"Resources/4.png", dxCommon->GetDev());
	SpriteCommonLoadTexture(spriteCommon, 15, L"Resources/5.png", dxCommon->GetDev());
	SpriteCommonLoadTexture(spriteCommon, 16, L"Resources/6.png", dxCommon->GetDev());
	SpriteCommonLoadTexture(spriteCommon, 17, L"Resources/7.png", dxCommon->GetDev());
	SpriteCommonLoadTexture(spriteCommon, 18, L"Resources/8.png", dxCommon->GetDev());
	SpriteCommonLoadTexture(spriteCommon, 19, L"Resources/9.png", dxCommon->GetDev());

	//FBX�̏�����
	FbxLoader::GetInstance()->Initialize(dxCommon->GetDev());

	// DirectX�����������@�����܂�

#pragma endregion


	//�X�v���C�g
	Sprite sprite;
	//�X�v���C�g�̐���
	sprite = SpriteCreate(dxCommon->GetDev(), winApp->window_width, winApp->window_height, 0, spriteCommon);
	//�X�v���C�g
	Sprite spriteWae;
	//�X�v���C�g�̐���
	spriteWae = SpriteCreate(dxCommon->GetDev(), winApp->window_width, winApp->window_height, 1, spriteCommon);
	//�X�v���C�g�̍X�V
	sprite.rotation = 0;
	sprite.position = { 0,0,0 };
	sprite.color = { 1,1,1,1 };
	SpriteUpdate(sprite, spriteCommon);
	sprite.size = { 1280,750 };
	//���_�o�b�t�@�ɔ��f
	SpriteTransferVertexBuffer(sprite);
	spriteWae.rotation = 0;
	spriteWae.position = { 0,0,0 };
	//spriteWae.color = { 1,1,0,1 };	
	SpriteUpdate(spriteWae, spriteCommon);
	spriteWae.size = { 1280,800 };
	SpriteTransferVertexBuffer(spriteWae);

	Sprite sprite1 = SpriteCreate(dxCommon->GetDev(), winApp->window_width, winApp->window_height, 11, spriteCommon);
	sprite1.rotation = 0;
	sprite1.position = { 1150,0,0 };
	SpriteUpdate(sprite1, spriteCommon);
	sprite1.size = { 50,50 };
	SpriteTransferVertexBuffer(sprite1);

	Sprite sprite2 = SpriteCreate(dxCommon->GetDev(), winApp->window_width, winApp->window_height, 11, spriteCommon);
	sprite2.rotation = 0;
	sprite2.position = { 1200,0,0 };
	SpriteUpdate(sprite2, spriteCommon);
	sprite2.size = { 50,50 };
	SpriteTransferVertexBuffer(sprite2);



	//�ϐ��錾
	/*���f������*/
	Model *playermodel = Model::Create();
	playermodel->CreateFromOBJ("player");
	Model *enemymodel = Model::Create();
	enemymodel->CreateFromOBJ("enemy");
	Model *titleModel = Model::Create();
	titleModel->CreateFromOBJ("title");
	Model *endModel = Model::Create();
	endModel->CreateFromOBJ("end");

	Model *backModel1 = Model::Create();
	backModel1->CreateFromOBJ("backb");

	Model *ropeModel = Model::Create();
	ropeModel->CreateFromOBJ("rope");

	/*Model *zeroModel = Model::Create();
	endModel->CreateFromOBJ("0");
	Model *oneModel = Model::Create();
	endModel->CreateFromOBJ("1");
	Model *twoModel = Model::Create();
	endModel->CreateFromOBJ("2");
	Model *threeModel = Model::Create();
	endModel->CreateFromOBJ("3");
	Model *fourModel = Model::Create();
	endModel->CreateFromOBJ("4");
	Model *fiveModel = Model::Create();
	endModel->CreateFromOBJ("5");
	Model *sixModel = Model::Create();
	endModel->CreateFromOBJ("6");
	Model *sevenModel = Model::Create();
	endModel->CreateFromOBJ("7");
	Model *eightModel = Model::Create();
	endModel->CreateFromOBJ("8");
	Model *nineModel = Model::Create();
	endModel->CreateFromOBJ("9");*/



	/*3d�I�u�W�F�N�g����*/
	//�v���C���[
	OBJobject *playerobj = OBJobject::Create();
	playerobj->SetModel(playermodel);
	playerobj->SetScale({ 1.0f,1.0f,1.0f });
	playerobj->SetPosition({ 0.0f,0.0f,0.0f });
	playerobj->SetRotation({ 0.0f,150.0f,0.0f });
	//�G
	const int enemynum = 20;
	int isenemy[enemynum] = { 0 };	//��ʓ��ɂ���G�̂���
	//isenemy[0] = 1;
	OBJobject *bardobj[enemynum];
	for (int i = 0; i < enemynum; i++)
	{
		bardobj[i] = OBJobject::Create();
		bardobj[i]->SetModel(enemymodel);
		bardobj[i]->SetScale({ 1.0f,1.0f,1.0f });
		bardobj[i]->SetPosition({ 0.0f,0.0f,0.0f });
		bardobj[i]->SetRotation({ 0.0f,90.0f,0.0f });
	}
	//UI�Ƃ�
	//�^�C�g��
	OBJobject *titleobj = OBJobject::Create();
	titleobj->SetModel(titleModel);
	titleobj->SetScale({ 20.0f,20.0f,20.0f });
	titleobj->SetPosition({ 0.0f,0.0f,0.0f });
	titleobj->SetRotation({ -90.0f, 0.0f, 0.0f });
	//�^�C�g���p�̔w�i
	OBJobject *backobj = OBJobject::Create();
	backobj->SetModel(backModel1);
	backobj->SetScale({ 10.0f,10.0f,10.0f });
	backobj->SetPosition({ 0.0f,0.0f,0.0f });
	backobj->SetRotation({ -90.0f, 0.0f, 0.0f });

	//�����
	OBJobject *endobj = OBJobject::Create();
	endobj->SetModel(endModel);
	endobj->SetScale({ 20.0f,20.0f,20.0f });
	endobj->SetPosition({ 0.0f,0.0f,0.0f });
	endobj->SetRotation({ -90.0f, 0.0f, 0.0f });

	//���[�v
	OBJobject *ropeobj = OBJobject::Create();
	ropeobj->SetModel(ropeModel);
	ropeobj->SetScale({ 2.0f,10.0f,2.0f });
	ropeobj->SetPosition({ 1.5f,60.0f,-3.0f });
	ropeobj->SetRotation({ 0.0f, 0.0f, 0.0f });



	//�I�u�W�F�N�g�̏����ݒ�̔��f
	playerobj->Update();
	for (int i = 0; i < enemynum; i++)
	{
		bardobj[i]->Update();
	}
	titleobj->Update();
	endobj->Update();
	backobj->Update();
	ropeobj->Update();

	const int spowntimeInit = 50;
	int spowntime = spowntimeInit;
	int gameTimerInit = 1800;
	int gameTimer = gameTimerInit;



	int respowntime = 0;
	float bardspeed = 0.8f;
	srand(time(NULL));	//���Ԃ𗐐��̃V�[�h��

	int point = 5;
	int score = 0;	//�X�R�A

	enum
	{
		title,
		game,
		end
	};
	int scene = title;


	bool hit = false;


	//���f�������w�肵�ăt�@�C���ǂݍ���
	FbxLoader::GetInstance()->LoadModelFromFile("cube");

	while (true)  // �Q�[�����[�v
	{
		// �u���b�N���̓y�[�W�E�����Q��
		// ���b�Z�[�W������H
		if (winApp->ProcessMessage())
		{
			break;
		}

		//DirectX���t���[�������@��������
		//-----���͂̍X�V����
		input->Update();


#pragma region �X�V����		


#pragma endregion

		//�v���C���[�̍X�V����
		XMFLOAT3 playerpos = playerobj->GetPosition();
		if (input->PushKey(DIK_W))
		{
			playerpos.y += 1.0f;
		}
		if (input->PushKey(DIK_S))
		{
			playerpos.y -= 1.0f;
		}
		if (input->PushKey(DIK_A))
		{
			playerpos.x -= 1.0f;
		}
		if (input->PushKey(DIK_D))
		{
			playerpos.x += 1.0f;
		}
		playerobj->SetPosition(playerpos);


		//3D�I�u�W�F�N�g�X�V
		playerobj->Update();

		//DirectX���t���[�������@�����܂�


		//�`�揈��
		dxCommon->PreDraw();


		//�X�v���C�g���ʃR�}���h
		SpriteCommonBeginDraw(spriteCommon, dxCommon->GetCmdList());
		//�X�v���C�g�`��
		//SpriteDraw(spriteWae, dxCommon->GetCmdList(), spriteCommon, dxCommon->GetDev());
		if (scene == game)
		{
			int time = gameTimer / 60;
			sprite1.texNumber = time / 10 + 10;
			if (sprite1.texNumber < 0)
			{
				sprite1.texNumber = 0;
			}
			sprite2.texNumber = time % 10 + 10;
			if (sprite2.texNumber < 0)
			{
				sprite2.texNumber = 0;
			}
			//SpriteDraw(sprite1, dxCommon->GetCmdList(), spriteCommon, dxCommon->GetDev());
			//SpriteDraw(sprite2, dxCommon->GetCmdList(), spriteCommon, dxCommon->GetDev());
		}

		//3D�I�u�W�F�N�g�`��O����
		OBJobject::PreDraw(dxCommon->GetCmdList());

		if (scene == title)
		{
		//	titleobj->Draw();
		//	playerobj->Draw();
		}
		else if (scene == game)
		{
			//�X�v���C�g���ʃR�}���h
			//SpriteCommonBeginDraw(spriteCommon, dxCommon->GetCmdList());
			//�X�v���C�g�`��
			//SpriteDraw(sprite1, dxCommon->GetCmdList(), spriteCommon, dxCommon->GetDev());
			//SpriteDraw(sprite2, dxCommon->GetCmdList(), spriteCommon, dxCommon->GetDev());

			//3D�I�u�W�F�N�g�`�揈��			
		//	playerobj->Draw();
		//	ropeobj->Draw();

			for (int i = 0; i < enemynum; i++)
			{
				if (isenemy[i] > 0)
				{
		//			bardobj[i]->Draw();
				}
			}
			if (hit)
			{
			//	backobj->Draw();
			}
		}
		else
		{
			SpriteCommonBeginDraw(spriteCommon, dxCommon->GetCmdList());

		//	SpriteDraw(sprite, dxCommon->GetCmdList(), spriteCommon, dxCommon->GetDev());
			//endobj->Draw();
		}

		playerobj->Draw();


		//3D�I�u�W�F�N�g�`��㏈��
		OBJobject::PostDraw();
		dxCommon->PostDraw();
	}


	winApp->Finalize();

	delete winApp;
	winApp = nullptr;
	//DirectX���
	delete dxCommon;
	//���͂̉��
	delete input;
	//3D�I�u�W�F�N�g���
	delete playerobj;

	//FBX�̉������
	FbxLoader::GetInstance()->Finalize();

	return 0;
}