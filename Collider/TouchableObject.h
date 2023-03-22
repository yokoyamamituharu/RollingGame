#pragma once
#include "ObjectObj.h"
class TouchableObject :
	public ObjectObj
{
public:
	TouchableObject() = default;
	~TouchableObject() = default;

	/// <summary>
	/// オブジェクト生成
	/// </summary>
	/// <returns></returns>
	static TouchableObject* Create(Model* model, XMFLOAT3 position = { 0,0,0 }, XMFLOAT3 rotation = { 0,0,0 }, XMFLOAT3 scale = { 1,1,1 });

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(Model * model);
};

