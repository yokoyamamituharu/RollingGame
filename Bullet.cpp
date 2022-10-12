#include "Bullet.h"

Model* Bullet::model = nullptr;

Bullet* Bullet::Create()
{
	//�C���X�^���X�𐶐�
	Bullet* bullet = new Bullet;
	
	//������
	if (!bullet->Initialize()) {
		delete bullet;
		assert(0);
		return nullptr;
	}	
	return bullet;
}

void Bullet::StaticInitialize()
{
	model = new Model();
	model->CreateFromOBJ("bullet");
}

void Bullet::Update()
{
	//float z = object->GetPosition().z;
	//object->SetPosition({ object->GetPosition().x, object->GetPosition().y,z + 1.0f });	
	//�ڕW�Ɍ������Đi��
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

	object->Update();
}

void Bullet::Draw()
{
	object->Draw();
}

bool Bullet::Initialize(XMFLOAT3 target,bool flag)
{
	//�e�p���f����ǂݍ���	
	//model = new Model();
	//model->CreateFromOBJ("bullet");
	//�I�u�W�F�N�g�̍쐬
	object = OBJobject::Create();
	object->SetModel(model);
	this->target = target;
	this->flag = flag;

	return true;
}