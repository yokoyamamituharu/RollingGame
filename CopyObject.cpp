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

	//������
	Initialize();

	//�R�s�[���̃��[���h���[�J���s����擾
	matWorldC = &obj->GetWorldMatrix();
	SetModel(obj->GetModel());	
}

void CopyObject::Update(ObjectObj* obj, Camera* cameraC)
{
	assert(camera);

	HRESULT result;

	// �萔�o�b�t�@�փf�[�^�]��
	ConstBufferDataB0* constMap = nullptr;
	result = constBuffB0->Map(0, nullptr, (void**)&constMap);
	//constMap->color = color;
	if (cameraC == nullptr) {
		constMap->mat = obj->GetWorldMatrix() * camera->GetMatView() * camera->GetMatProjection();	// �s��̍���
	}
	else {
		constMap->mat = obj->GetWorldMatrix() * cameraC->GetMatView() * cameraC->GetMatProjection();	// �s��̍���
	}
	constBuffB0->Unmap(0, nullptr);

	//constBuffB1�̒萔�o�b�t�@�f�[�^�]������
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
	


	//�`�揈��

	// nullptr�`�F�b�N
	assert(device);
	assert(ObjectObj::commandList);

	// ���_�o�b�t�@�̐ݒ�
	commandList->IASetVertexBuffers(0, 1, &modelData->vbView);
	// �C���f�b�N�X�o�b�t�@�̐ݒ�
	commandList->IASetIndexBuffer(&modelData->ibView);

	// �f�X�N���v�^�q�[�v�̔z��
	ID3D12DescriptorHeap* ppHeaps[] = { modelData->descHeap.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	// �萔�o�b�t�@�r���[���Z�b�g
	commandList->SetGraphicsRootConstantBufferView(0, constBuffB0->GetGPUVirtualAddress());
	commandList->SetGraphicsRootConstantBufferView(1, constBuffB1->GetGPUVirtualAddress());
	// �V�F�[�_���\�[�X�r���[���Z�b�g
	commandList->SetGraphicsRootDescriptorTable(2, modelData->gpuDescHandleSRV);
	// �`��R�}���h
	//commandList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
	commandList->DrawIndexedInstanced((UINT)modelData->indices.size(), 1, 0, 0, 0);
}
