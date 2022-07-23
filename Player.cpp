#include "Player.h"

void Player::Initialize(Input* input, InputMouse* mouse)
{
	assert(input);
	assert(mouse);
	this->input = input;
	this->mouse = mouse;
	//�v���C���[�p���f����ǂݍ���
	playermodel = new Model();
	playermodel->CreateFromOBJ("player");
	playerSpheremodel = new Model();
	playerSpheremodel->CreateFromOBJ("playerSphere");

	//�I�u�W�F�N�g�̍쐬
	object = OBJobject::Create();
	object->SetModel(playermodel);
}

void Player::Update()
{
	if (mouse->PushMouse(MouseDIK::M_LEFT)) {
		isSphere = true;
	}
	else {
		isSphere = false;
	}

	if (isSphere) {
		//���f����ς���
		object->SetModel(playerSpheremodel);
		//�}�E�X�̉��ւ̈ړ��ʂ�ۑ��i���ɉ�����΁{�A��ɂ�����΁\�i0�ȉ��ɂ͂Ȃ�Ȃ��j�j
		rollingSpeed += mouse->MoveMouseVector('y') / 30;
		if (rollingSpeed < 0) {
			rollingSpeed = 0;
		}
		//�}�E�X�̈ړ��ʂ��v���C���[�̉�]���x�ɂ�����
		spiralSpeed.z = rollingSpeed;
		//�}�E�X�𗣂����Ƃ��A�ړ��ʂ���������v���C���[�𒼐i������

		//���̎��̃v���C���[�̉�]���x�̓v���C���[�̈ړ����x�Ɉˑ�

	}
	else if (rollingSpeed > 0){
		MoveVector({ 0,0,rollingSpeed });
		rollingSpeed -= 1;
		if (rollingSpeed < 0) {
			rollingSpeed = 0;
		}
	}
	else{
		rollingSpeed = 0.0f;
		spiralSpeed.z = 0;
		object->SetRotation({ object->GetRotation().x, object->GetRotation().y, 0.0f, });
		object->SetModel(playermodel);
	}
	SpiralVector(spiralSpeed);

	//�I�u�W�F�N�g�̃A�b�v�f�[�g
	object->Update();
}
