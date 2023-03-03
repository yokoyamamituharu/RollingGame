#pragma once
#include "ObjectObj.h"


//�p�[�e�B�N��1��
class ParticleTubu
{
public:
	ParticleTubu();
	~ParticleTubu();

public:
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

public:
	std::unique_ptr<ObjectObj> obj = nullptr;
};
