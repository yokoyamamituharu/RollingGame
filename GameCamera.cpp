#include "GameCamera.h"
#include "Input.h"
#include "InputMouse.h"
#include "Useful.h"

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
	assert(playerObj);
	//�}�E�X�ŃJ�������삷�邩�̐؂�ւ�����
	if (Input::GetInstance()->TriggerKey(DIK_F2)) {
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

	//if (Input::GetInstance()->TriggerKey(DIK_C)) {
	//	if (showCorsl == true) {
	//		showCorsl = false;
	//	}
	//	else {
	//		showCorsl = true;
	//	}		
	//}

	if (isSetMousePoint == false) {
		showCorsl = true;
	}
	ShowCursor(showCorsl);

	//�J�����̌����A�ʒu���v�Z
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	//XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rote.y));
	movement = XMVector3TransformNormal(movement, matRot);
	movement *= XMVECTOR{ -1, -1, -1 };
	SetEye(playerObj->GetPosition() + (movement * 20));
	eye.y += 7;	
	target = playerObj->GetPosition();
	target.y += 3;

	//�r���[�s��̍X�V
	UpdateViewMatrix();
	// �r���[�v���W�F�N�V�����̍���
	matViewProjection = matView * matProjection;
}
