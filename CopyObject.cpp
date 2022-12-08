#include "CopyObject.h"

CopyObject* CopyObject::Create(ObjectObj* obj)
{
	CopyObject* ins = new CopyObject;
	ins->InitializeC(obj);
	return ins;
}

void CopyObject::InitializeC(ObjectObj* obj)
{
	assert(obj);

	//初期化
	Initialize();

	//コピー元のワールドローカル行列を取得
	matWorldC = &obj->GetWorldMatrix();
	SetModel(obj->GetModel());	
}

void CopyObject::Update(ObjectObj* obj, Camera* cameraC)
{
	assert(camera);

	HRESULT result;

	// 定数バッファへデータ転送
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	//constMap->color = color;
	if (cameraC == nullptr) {
		constMap->mat = obj->GetWorldMatrix() * camera->GetMatView() * camera->GetMatProjection();	// 行列の合成
	}
	else {
		constMap->mat = obj->GetWorldMatrix() * cameraC->GetMatView() * cameraC->GetMatProjection();	// 行列の合成
	}
	constBuffB0->Unmap(0, nullptr);

	//constBuffB1の定数バッファデータ転送処理
	ConstBufferDataB1* constMap1 = nullptr;
	result = constBuffB1->Map(0, nullptr, (void**)&constMap1);
	constMap1->ambient = modelData->material.ambient;
	constMap1->diffuse = modelData->material.diffuse;
	constMap1->specular = modelData->material.specular;
	constMap1->blackOutAlpha = modelData->material.blackOutAlpha;
	constBuffB1->Unmap(0, nullptr);
}

void CopyObject::Draw()
{
	


	//描画処理

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
