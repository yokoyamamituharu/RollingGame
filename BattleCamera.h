#pragma once
#include "Camera.h"
#include "ObjectObj.h"
#include "Player.h"

class BattleCamera :
	public Camera
{
public:
	static BattleCamera* Create();

	void Update()override;

	void SetPlayer(ObjectObj* obj) { playerObj = obj; }
	void SetPlayer(Player* ptr) { player = ptr; }

private:
	//�J�[�\���̌Œ�
	bool isSetMousePoint = false;
	//�v���C���[�̃|�C���^�[
	ObjectObj* playerObj = nullptr;
	Player* player = nullptr;
	bool showCorsl = false;

	XMFLOAT3 dirEye = { 0,0,0 };
	XMFLOAT3 dirTarget = { 0,0,0 };
};