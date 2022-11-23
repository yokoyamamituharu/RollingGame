#include "GameCamera.h"
#include "Input.h"
#include "InputMouse.h"

GameCamera* GameCamera::Create()
{
	//�J�����N���X�̃C���X�^���X�𐶐�
	GameCamera* ins = new GameCamera;
	//����������
	ins->Initialize();
	//���������C���X�^���X��Ԃ�
	return ins;
}

void GameCamera::Update()
{
	//�}�E�X�ŃJ�������삷�邩�̐؂�ւ�����
	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
		if (isSetMousePoint == true)  isSetMousePoint = false;
		else  isSetMousePoint = true;
	}
	//�}�E�X�ŃJ��������//�E�B���h�E���A�N�e�B�u��ԂȂ珈��
	if (WinApp::GetInstance()->GetHwnd() == GetActiveWindow() && isSetMousePoint == true) {
		matRot *= XMMatrixRotationY(0.8f * InputMouse::GetInstance()->MoveMouseVector('x') / 1000);
		InputMouse::GetInstance()->SetCenterCoursolPos();
	}
	//�L�[�ŃJ��������
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		matRot *= XMMatrixRotationY(0.02f);
	}
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		matRot *= XMMatrixRotationY(-0.02f);
	}

	//�J�����̌����A�ʒu���v�Z
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	//XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rote.y));
	movement = XMVector3TransformNormal(movement, matRot);
	movement *= XMVECTOR{ -1, -1, -1 };
	SetEye(playerObj->GetPosition() + (movement * 80));
	eye.y = 20;
	target = playerObj->GetPosition();


	//�r���[�s��̍X�V
	UpdateViewMatrix();
	// �r���[�v���W�F�N�V�����̍���
	matViewProjection = matView * matProjection;
}
