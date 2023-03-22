#include "TouchableObject.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"

TouchableObject * TouchableObject::Create(Model * model, XMFLOAT3 position, XMFLOAT3 rotation, XMFLOAT3 scale)
{
	// オブジェクトのインスタンスを生成
	TouchableObject* instance = new TouchableObject();
	if (instance == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!instance->Initialize(model)) {
		delete instance;
		assert(0);
	}

	instance->SetPosition(position);
	instance->SetRotation(rotation);
	instance->SetScale(scale);

	return instance;
}

bool TouchableObject::Initialize(Model * model)
{
	if (!ObjectObj::Initialize())
	{
		return false;
	}

	SetModel(model);

	// コライダーの追加
	MeshCollider* collider = new MeshCollider;
	SetCollider(collider);
	collider->ConstructTriangles(model);
	collider->SetAttribute(COLLISION_ATTR_LANDSHAPE);

	return true;
}
