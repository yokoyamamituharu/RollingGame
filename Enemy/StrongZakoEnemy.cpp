#include "StrongZakoEnemy.h"

Model* StrongZakoEnemy::strongEnemyModel = nullptr;

void StrongZakoEnemy::Initialize(int filedFlag, Camera* camera, XMFLOAT3 pos, bool isTarget, XMFLOAT3 targetPos1, XMFLOAT3 targetPos2)
{
	this->isFiled = filedFlag;
	this->camera = camera;
	this->isTarget = isTarget;
	this->targetPos.m128_f32[0] = targetPos1.x;
	this->targetPos.m128_f32[1] = targetPos1.y;
	this->targetPos.m128_f32[2] = targetPos1.z;
	//�I�u�W�F�N�g�̍쐬
	object = OBJobject::Create();
	object->SetModel(strongEnemyModel);

	//�O�p�̓G�Ƃ��ăC���X�^���X���������ꂽ�ꍇ
	if (filedFlag == 1) {
		object->SetPosition(pos);
		object->SetScale({ 4.0f,4.0f, 4.0f });

		//���̓G�����V�[���Ɉړ��������Ɏ����Ă��鏬�G�̏���ǉ�
		int maxEnemyNum = rand() % 2 + 5;
		for (int i = 0; i < maxEnemyNum; i++)
		{
			//�G�����X�g�ɒǉ����Ă���
			std::unique_ptr<StrongZakoEnemy> newEnemyZako = std::make_unique<StrongZakoEnemy>();
			newEnemyZako->Initialize(FIELD_IN, camera);
			//���X�g�ɓo�^
			enemies.push_back(std::move(newEnemyZako));
		}

		//�ړI�n
		if (isTarget == true) {
			//�ړ�����������v�Z����
			XMVECTOR pos1;
			pos1.m128_f32[0] = object->GetPosition().x;
			pos1.m128_f32[1] = object->GetPosition().y;
			pos1.m128_f32[2] = object->GetPosition().z;
			targetVec = pos1 - this->targetPos;
			targetVec = XMVector3Normalize(targetVec);
			targetVec.m128_f32[1] = 0;//������0�ɂ��Ȃ��ƃv���C���[�ƓG��Y���W�̂���œG�̓ːi�����������
		}
	}
	//���p�̓G�Ƃ��ăC���X�^���X���������ꂽ�ꍇ
	else if (filedFlag == 2) {
		//���W�̃Z�b�g
		float x = rand() % 200 - 100;
		float z = rand() % 200 - 100;
		object->SetPosition({ x,groundInPos + 6.0f,z });
		//�T�C�Y�̃Z�b�g
		object->SetScale({ 4.0f,4.0f, 4.0f });
		object->SetScale({ 10,10,10 });
	}


}

void StrongZakoEnemy::Update()
{
	if (isFiled == FIELD_OUT && isAction > 0) {
		//HP�ɂ���ăT�C�Y��ς���
		scale = hp / maxHp;
		if (scale <= 0) {
			scale = 1;
			object->SetRotation({ 180,90,0 });
		}
		object->SetScale(maxScale * scale);

		//�ړI�n�ւƍs������
		if (isTarget == true) {
			XMVECTOR pos;
			pos.m128_f32[0] = object->GetPosition().x;
			pos.m128_f32[1] = object->GetPosition().y;
			pos.m128_f32[2] = object->GetPosition().z;
			pos -= targetVec * 1;
			object->SetPosition({ pos.m128_f32[0],pos.m128_f32[1] ,pos.m128_f32[2] });
		}
	}
	else if (isFiled == FIELD_IN/* && isAction > 0*/) {

		
		MoveMae();
	}
	//�I�u�W�F�N�g�̍X�V
	object->Update();
}

void StrongZakoEnemy::CreateStrongEnemyModel()
{
	//�G�p���f����ǂݍ���
	strongEnemyModel = Model::Create("strongEnemy");
}

void StrongZakoEnemy::Move()
{
}

void StrongZakoEnemy::MoveMae()
{


	if (Nitenkan(player->object->GetPosition(), object->GetPosition()) <= 10.0f) {
		maeTime++;
	}
	else {
		maeTime--;
		if (maeTime < 0) {
			maeTime = 0;
		}
	}

	if (maeTime > 20) {
		Dossun();
		maeTime = 0;
	}

	if (dossunFlag == false)
	{
		object->VecSetPosition({ 0,0,0.3 });
	}

}

void StrongZakoEnemy::Dossun()
{
	if (dosuntime1 < 60) {
		dosuntime1++;
		object->VecSetPosition({ 0,1.1f,0 });
	}
	else if (dosuntime2 < 60) {
		dosuntime2++;
		object->VecSetPosition({ 0,-1.1f,0 });
	}
	else {
		dosuntime1 = 0;
		dosuntime2 = 0;

	}




}
