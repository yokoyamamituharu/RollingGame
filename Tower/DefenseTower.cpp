#include "DefenseTower.h"
#include "Input.h"

float Kyori(XMFLOAT3 pos1, XMFLOAT3 pos2)
{
	float distance = sqrtf(((pos1.x - pos2.x) * (pos1.x - pos2.x)) +
		((pos1.z - pos2.z) * (pos1.z - pos2.z)));

	return distance;
}

DefenseTower* DefenseTower::Create()
{
	//�C���X�^���X�𐶐�
	DefenseTower* defenseTower = new DefenseTower;

	//����������
	if (!defenseTower->Initialize()) {
		delete defenseTower;
		assert(0);
		return nullptr;
	}

	defenseTower->object->SetScale({ 7, 7, 7 });
	defenseTower->object->SetPosition({ 0, 10, 0 });
	return defenseTower;
}

void DefenseTower::Update(std::list<std::shared_ptr<EnemyZako>>& enemies)
{
	//�t���O���������e������
	bullets.remove_if([](std::unique_ptr<Bullet>& bullet) {return bullet->IsDead(); });


	//���m�͈͂ɓG����������U���J�n
	//�^�[�Q�b�g�̃G�l�~�[����i�|����Ă���j��������V���ȃ^�[�Q�b�g��T��
	if (targetEnemy.expired()) {
		for (std::shared_ptr<EnemyZako>& enemy : enemies) {
			if (Kyori(object->GetPosition(), enemy->object->GetPosition()) <= 100.0f &&
				enemy->GetHp() > 0) {
				attackFlag = true;
				targetEnemy = enemy;
				break;
			}
			else {
				attackFlag = false;
			}
		}
	}	

	//�������ł�������������̂��ǂ����͂킩��˂���
	//std::shared_ptr<int> ptr = targetEnemy.lock();

	//�e�̐��������A�e��G�̂����Ƃ��ɔ���
	if (attackFlag == true) {
		if (interval <= 0) {
			//�{���Ȃ�G�̃}�l�[�W���[�Ƃ�����Ώۂ̓G�̂��擾
			DirectX::XMFLOAT3 targetpos = targetEnemy.lock()->object->GetPosition();
			targetpos.y = 0;
			std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
			newBullet->Initialize(object->GetPosition(),targetpos, true);
			bullets.push_back(std::move(newBullet));
			interval = maxInterval;
		}
		else {
			interval--;
		}
	}

	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		object->VecSetPosition({ +1,0,0 });
	}
	if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		object->VecSetPosition({ -1,0,0 });
	}

	if (Input::GetInstance()->PushKey(DIK_UP)) {
		object->VecSetPosition({ 0,0,+1 });
	}
	if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		object->VecSetPosition({ 0,0,-1 });
	}


	//�Ƃ肠�����v���C���[�ɂނ��Č���

	object->Update();

	for (std::unique_ptr<Bullet>& bullet : bullets) {
		if (targetEnemy.expired() == false) {
			if (5.0f >= Kyori(bullet->object->GetPosition(), targetEnemy.lock()->object->GetPosition())) {
				//targetEnemy.lock()->Damege(1);
			}
		}
		bullet->Update();
	}

	//�^�[�Q�b�g�̔j���̏���
	if (targetEnemy.expired() ==false) {
		if (Kyori(object->GetPosition(), targetEnemy.lock()->object->GetPosition()) > 100.0f ||
			targetEnemy.lock()->GetHp() < 0) {
			targetEnemy.reset();
			attackFlag = false;
		}
	}
}

void DefenseTower::Draw()
{
	object->Draw();
	for (std::unique_ptr<Bullet>& bullet : bullets) {
		bullet->Draw();
	}
}

bool DefenseTower::Initialize()
{
	//�^���[�p���f����ǂݍ���
	model = Model::Create("defenseTower");
	//�I�u�W�F�N�g�̍쐬
	object = OBJobject::Create();
	object->SetModel(model);

	return true;
}