#pragma once
#include "Camera.h"
#include "ObjectObj.h"

class BattleCamera :
	public Camera
{
public:
	static BattleCamera* Create();

	void Update()override;

	void SetPlayer(ObjectObj* obj) { playerObj = obj; }
private:
	//�J�[�\���̌Œ�
	bool isSetMousePoint = true;
	//�v���C���[�̃|�C���^�[
	ObjectObj* playerObj = nullptr;
	bool showCorsl = false;
};