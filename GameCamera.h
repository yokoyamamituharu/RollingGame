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
	//カーソルの固定
	bool isSetMousePoint = true;
	//プレイヤーのポインター
	ObjectObj* playerObj = nullptr;
};