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
	//カーソルの固定
	bool isSetMousePoint = true;
	//プレイヤーのポインター
	ObjectObj* playerObj = nullptr;
	bool showCorsl = false;
};