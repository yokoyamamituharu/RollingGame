#pragma once
#include "Camera.h"
#include "ObjectOBJ.h"

class GameCamera :
	public Camera
{
public:
	static GameCamera* Create();

	void Update()override;

	void SetPlayer(ObjectObj* obj) { playerObj = obj; }
private:
	//�J�[�\���̌Œ�
	bool isSetMousePoint = false;
	//�v���C���[�̃|�C���^�[
	ObjectObj* playerObj = nullptr;
	bool showCorsl = false;
};