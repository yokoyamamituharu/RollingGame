#include "Bullet.h"

Model* Bullet::model = nullptr;

Bullet* Bullet::Create()
{
	//インスタンスを生成
	Bullet* bullet = new Bullet;
	
	//初期化
	if (!bullet->Initialize()) {
		delete bullet;
		assert(0);
		return nullptr;
	}	
	return bullet;
}

void Bullet::StaticInitialize()
{
	model = Model::Create("bullet");
}

void Bullet::Update()
{
	//float z = object->GetPosition().z;
	//object->SetPosition({ object->GetPosition().x, object->GetPosition().y,z + 1.0f });	
	//目標に向かって進む
	float speed = 10.0f;
	float posX = object->GetPosition().x;
	float posY = object->GetPosition().y;
	float posZ = object->GetPosition().z;
	float distanceX = 0;
	float distanceZ = 0;
	distanceX = posX - target.x;
	distanceZ = posZ - target.z;
	posX -= distanceX / speed;
	posZ -= distanceZ / speed;
	object->SetPosition(XMFLOAT3(posX, posY, posZ));

	//時間がきたら消す
	timer++;
	if (timer > 60) {
		dead = true;
	}

	object->Update();
}

void Bullet::Draw()
{
	object->Draw();
}

bool Bullet::Initialize(XMFLOAT3 pos,XMFLOAT3 target,bool flag)
{
	//弾用モデルを読み込み	
	//model = Model::Create("bullet");
	//オブジェクトの作成
	object = OBJobject::Create();
	object->SetModel(model);
	object->SetPosition(pos);
	this->target = target;
	this->flag = flag;

	return true;
}

Bullet::Bullet()
{
}


Bullet::~Bullet()
{
	delete(object);
}
