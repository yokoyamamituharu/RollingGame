﻿#include "ObjectObj.h"
#include <d3dcompiler.h>
#include <DirectXTex.h>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include "CollisionManager.h"
#include "BaseCollider.h"

using namespace std;

#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using namespace Microsoft::WRL;

/// <summary>
/// 静的メンバ変数の実体
/// </summary>
const float ObjectObj::radius = 5.0f;				// 底面の半径
const float ObjectObj::prizmHeight = 8.0f;			// 柱の高さ
ID3D12Device* ObjectObj::device = nullptr;
UINT ObjectObj::descriptorHandleIncrementSize = 0;
ID3D12GraphicsCommandList* ObjectObj::commandList = nullptr;
ComPtr<ID3D12RootSignature> ObjectObj::rootsignature;
ComPtr<ID3D12PipelineState> ObjectObj::pipelinestate;
Camera* ObjectObj::camera = nullptr;


bool ObjectObj::StaticInitialize(ID3D12Device* device, Camera* camera)
{
	// nullptrチェック
	assert(device);
	assert(camera);

	ObjectObj::device = device;

	ObjectObj::camera = camera;

	// パイプライン初期化
	InitializeGraphicsPipeline();


	return true;
}

void ObjectObj::PreDraw(ID3D12GraphicsCommandList* commandList)
{
	// PreDrawとPostDrawがペアで呼ばれていなければエラー
	assert(ObjectObj::commandList == nullptr);

	// コマンドリストをセット
	ObjectObj::commandList = commandList;

	// パイプラインステートの設定
	commandList->SetPipelineState(pipelinestate.Get());
	// ルートシグネチャの設定
	commandList->SetGraphicsRootSignature(rootsignature.Get());
	// プリミティブ形状を設定
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void ObjectObj::PostDraw()
{
	// コマンドリストを解除
	ObjectObj::commandList = nullptr;
}

ObjectObj* ObjectObj::Create(Model* model,XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale)
{
	// 3Dオブジェクトのインスタンスを生成
	ObjectObj* objectObj = new ObjectObj;
	if (objectObj == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!objectObj->Initialize(model)) {
		delete objectObj;
		assert(0);
		return nullptr;
	}	

	objectObj->SetPosition(position);
	objectObj->SetRotation(rotation);
	objectObj->SetScale(scale);

	return objectObj;
}



bool ObjectObj::InitializeGraphicsPipeline()
{
	HRESULT result = S_FALSE;
	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob;	// ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/OBJVertexShader.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "vs_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&vsBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// ピクセルシェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"Resources/shaders/OBJPixelShader.hlsl",	// シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // インクルード可能にする
		"main", "ps_5_0",	// エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0,
		&psBlob, &errorBlob);
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string errstr;
		errstr.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			errstr.begin());
		errstr += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(errstr.c_str());
		exit(1);
	}

	// 頂点レイアウト
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{ // xy座標(1行で書いたほうが見やすい)
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // 法線ベクトル(1行で書いたほうが見やすい)
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{ // uv座標(1行で書いたほうが見やすい)
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプラインの流れを設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC gpipeline{};
	gpipeline.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	gpipeline.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク
	gpipeline.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // 標準設定
	// ラスタライザステート
	gpipeline.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	//gpipeline.RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	//gpipeline.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	// デプスステンシルステート
	gpipeline.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);

	// レンダーターゲットのブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blenddesc{};
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;	// RBGA全てのチャンネルを描画
	blenddesc.BlendEnable = true;
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ブレンドステートの設定
	gpipeline.BlendState.RenderTarget[0] = blenddesc;

	// 深度バッファのフォーマット
	gpipeline.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	// 頂点レイアウトの設定
	gpipeline.InputLayout.pInputElementDescs = inputLayout;
	gpipeline.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定（三角形）
	gpipeline.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	gpipeline.NumRenderTargets = 1;	// 描画対象は1つ
	gpipeline.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM; // 0～255指定のRGBA
	gpipeline.SampleDesc.Count = 1; // 1ピクセルにつき1回サンプリング

	// デスクリプタレンジ
	CD3DX12_DESCRIPTOR_RANGE descRangeSRV;
	descRangeSRV.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0); // t0 レジスタ

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootparams[3];
	rootparams[0].InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[1].InitAsConstantBufferView(1, 0, D3D12_SHADER_VISIBILITY_ALL);
	rootparams[2].InitAsDescriptorTable(1, &descRangeSRV, D3D12_SHADER_VISIBILITY_ALL);

	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャの設定
	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_0(_countof(rootparams), rootparams, 1, &samplerDesc, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> rootSigBlob;
	// バージョン自動判定のシリアライズ
	result = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	// ルートシグネチャの生成
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootsignature));
	if (FAILED(result)) {
		return result;
	}

	gpipeline.pRootSignature = rootsignature.Get();

	// グラフィックスパイプラインの生成
	result = device->CreateGraphicsPipelineState(&gpipeline, IID_PPV_ARGS(&pipelinestate));

	if (FAILED(result)) {
		return result;
	}

	return true;
}


ObjectObj::ObjectObj()
{
}

ObjectObj::~ObjectObj()
{
	if (collider) {
		CollisionManager::GetInstance()->RemoveCollider(collider);
		delete collider;
	}
}

bool ObjectObj::Initialize(Model* model)
{
	// nullptrチェック
	assert(device);

	HRESULT result;
	// 定数バッファの生成
	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB0) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB0));

	result = device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), 	// アップロード可能
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferDataB1) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffB1));

	//オブジェクトの初期位置
	position = { 0,0,0 };
	//オブジェクトの初期サイズ
	scale = { 1.0f,1.0f,1.0f };
	//オブジェクトの初期回転角度
	rotation = { 0.0f,0.0f,0.0f };

	//モデルデータを渡されていたらモデルをセット
	if (model != nullptr) {
		modelData = model;
	}

	return true;
}

void ObjectObj::Update()
{
	assert(camera);
	assert(modelData);

	HRESULT result;

	//行列の更新
	UpdateWorldMatrix();

	// 定数バッファへデータ転送
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	//constMap->color = color;
	if (!constMap) {
		int a = 0;
	}
	constMap->mat = matWorld * camera->GetMatView() * camera->GetMatProjection();	// 行列の合成
	constBuffB0->Unmap(0, nullptr);

	//constBuffB1の定数バッファデータ転送処理
	ConstBufferDataB1* constMap1 = nullptr;
	result = constBuffB1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = modelData->material.ambient;
	constMap1->diffuse = modelData->material.diffuse;
	constMap1->specular = modelData->material.specular;
	constMap1->blackOutAlpha = modelData->material.blackOutAlpha;
	constBuffB1->Unmap(0, nullptr);

	// 当たり判定更新
	if (collider) {
		collider->Update();
	}
}

void ObjectObj::Draw()
{
	// nullptrチェック
	assert(device);
	assert(ObjectObj::commandList);

	// 頂点バッファの設定
	commandList->IASetVertexBuffers(0, 1, &modelData->vbView);
	// インデックスバッファの設定
	commandList->IASetIndexBuffer(&modelData->ibView);

	// デスクリプタヒープの配列
	ID3D12DescriptorHeap* ppHeaps[] = { modelData->descHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// 定数バッファビューをセット
	commandList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());
	// シェーダリソースビューをセット
	commandList->SetGraphicsRootDescriptorTable(2, modelData->gpuDescHandleSRV);
	// 描画コマンド
	//commandList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
	commandList->DrawIndexedInstanced((UINT)modelData->indices.size(), 1, 0, 0, 0);
}

void ObjectObj::UpdateWorldMatrix()
{
	assert(camera);

	XMMATRIX matScale, matRot, matTrans;

	// スケール、回転、平行移動行列の計算
	matScale = XMMatrixScaling(scale.x, scale.y, scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);

	matWorld = XMMatrixIdentity(); // 変形をリセット
	matWorld *= matScale; // ワールド行列にスケーリングを反映
	matWorld *= matRot; // ワールド行列に回転を反映
	matWorld *= matTrans; // ワールド行列に平行移動を反映

	// 親オブジェクトがあれば
	if (parent != nullptr) {
		// 親オブジェクトのワールド行列を掛ける
		matWorld *= parent->matWorld;
	}
}

XMMATRIX ObjectObj::GetMatRot()
{
	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	return matRot;
}

XMMATRIX ObjectObj::GetWorldMatrix()
{
	return matWorld;
}

void ObjectObj::SetCollider(BaseCollider* collider)
{
	collider->SetObject(this);
	this->collider = collider;
	// コリジョンマネージャに追加
	CollisionManager::GetInstance()->AddCollider(collider);

	UpdateWorldMatrix();
	collider->Update();
}

void ObjectObj::SetModel(Model* model)
{
	modelData = model;
}
