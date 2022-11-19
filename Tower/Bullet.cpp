#include "Bullet.h"
#include "ModelManager.h"
#include "safe_delete.h"

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

void Bullet::Update()
{
	//float z = object->GetPosition().z;
	//object->SetPosition({ object->GetPosition().x, object->GetPosition().y,z + 1.0f });	
	//�ڕW�Ɍ������Đi��
	float speed = 5.0f;
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

	//���Ԃ����������
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
	ModelManager::GetModel("bullet");
	//�I�u�W�F�N�g�̍쐬
	object = ObjectObj::Create();
	object->SetModel(ModelManager::GetModel("bullet"));
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
	safe_delete(object);
}
