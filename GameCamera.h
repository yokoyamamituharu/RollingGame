#pragma once
#include "Camera.h"
#include "ObjectObj.h"

class GameCamera :
	public Camera
{
public:
	static GameCamera* Create();

	void Update()override;

	void SetPlayer(ObjectObj* obj) { playerObj = obj; }

	void SetKeisu(float keisu) { this->keisu = keisu; }

	void SetFlag(bool flag) { this->flag = flag; }
private:
	//�J�[�\���̌Œ�
	bool isSetMousePoint = true;
	//�v���C���[�̃|�C���^�[
	ObjectObj* playerObj = nullptr;
	bool showCorsl = false;
	float keisu = 20;
	bool flag = false;
};