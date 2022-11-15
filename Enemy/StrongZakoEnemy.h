#pragma once
#include "Enemy.h"
#include "EnemyZako.h"
class StrongZakoEnemy :
	public EnemyZako
{
private:
	//DirectX���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	void Initialize(int filedFlag, Camera* camera, XMFLOAT3 pos = { 0,0,0 }, bool isTarget = false,XMFLOAT3 targetPos1 = { 0,0,0 }, XMFLOAT3 targetPos2 = { 0,0,0 })override;

	void Update()override;

	static void CreateStrongEnemyModel();


	//�O�ɐi��
	void Move();

	void MoveMae();

	//�v���C���[����莞�ԋ߂��ɂ�����ǂ�����
	void Dossun();

private:
	//int hp = 10;
	XMFLOAT3 maxScale = { 4,4,4 };
	OBJobject* tuyoObject = nullptr;

	static Model* strongEnemyModel;

	int maeTime = 0;

	int dosuntime1 = 0;
	int dosuntime2 = 0;

	bool dossunFlag = false;
};

