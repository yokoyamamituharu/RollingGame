#include "DefenseTower.h"
#include "Input.h"

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

void DefenseTower::Update(const Player* player)
{
	//�t���O���������e������
	bullets.remove_if([](std::unique_ptr<Bullet>& bullet) {return bullet->IsDead(); });


	//���m�͈͂ɓG����������U���J�n
	if (Input::GetInstance()->TriggerKey(DIK_1)) {
		attackFlag = true;
	}

	if (Input::GetInstance()->TriggerKey(DIK_2)) {
		attackFlag = false;
	}
	//�e�̐��������A�e��G�̂����Ƃ��ɔ���
	if (attackFlag == true) {
		if (interval <= 0) {
			//�{���Ȃ�G�̃}�l�[�W���[�Ƃ�����Ώۂ̓G�̂��擾
			DirectX::XMFLOAT3 target = player->object->GetPosition();
			std::unique_ptr<Bullet> newBullet = std::make_unique<Bullet>();
			newBullet->Initialize(object->GetPosition(), player->object->GetPosition(), true);
			bullets.push_back(std::move(newBullet));
			interval = maxInterval;
		}
		else {
			interval--;
		}
	}

	//�Ƃ肠�����v���C���[�ɂނ��Č���

	object->Update();

	for (std::unique_ptr<Bullet>& bullet : bullets) {
		bullet->Update();
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
	model = new Model();
	model->CreateFromOBJ("defenseTower");
	//�I�u�W�F�N�g�̍쐬
	object = OBJobject::Create();
	object->SetModel(model);

	return true;
}