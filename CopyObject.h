#pragma once
#include "ObjectOBJ.h"
class CopyObject :
	public ObjectObj
{
public:
	static CopyObject* Create(ObjectObj* obj);

	void InitializeC(ObjectObj* obj);

	void Update(ObjectObj* obj, Camera* cameraC = nullptr);

	void Draw()override;

public:
	void SetMat(XMMATRIX matWorld) { matWorldC = &matWorld; };

private:
	// ローカルワールド変換行列
	XMMATRIX* matWorldC;
};

