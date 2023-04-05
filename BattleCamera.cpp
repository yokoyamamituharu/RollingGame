#include "BattleCamera.h"
#include "Input.h"
#include "InputMouse.h"
#include "Useful.h"

BattleCamera* BattleCamera::Create()
{
	//�J�����N���X�̃C���X�^���X�𐶐�
	BattleCamera* ins = new BattleCamera;
	//����������
	ins->Initialize();
	//���������C���X�^���X��Ԃ�
	return ins;
}

void BattleCamera::Update()
{
	eye = eye - dirEye;
	target = target - dirTarget;
	dirEye = {0.0f,0.0f ,0.0f };
	dirTarget = { 0.0f,0.0f ,0.0f };

	assert(playerObj);
	//�}�E�X�ŃJ�������삷�邩�̐؂�ւ�����
	if (Input::GetInstance()->TriggerKey(DIK_F2)) {
		if (isSetMousePoint == true)  isSetMousePoint = false;
		else  isSetMousePoint = true;
	}
	if (InputMouse::GetInstance()->PushMouse(MouseDIK::M_LEFT)) {
		isSetMousePoint = false;
	}
	else {
		isSetMousePoint = true;
	}

	//�}�E�X�ŃJ��������//�E�B���h�E���A�N�e�B�u��ԂȂ珈��
	if (WinApp::GetInstance()->GetHwnd() == GetActiveWindow() && isSetMousePoint == true) {
		InputMouse::GetInstance()->SetCenterCoursolPos();
	}


	RECT rcClip;           // new area for ClipCursor
	RECT rcOldClip;        // previous area for ClipCursor

	// Record the area in which the cursor can move. 

	GetClipCursor(&rcOldClip);

	// Get the dimensions of the application's window. 

	GetWindowRect(WinApp::GetInstance()->GetHwnd(), &rcClip);

	// Confine the cursor to the application's window. 

	//ClipCursor(&rcClip);

	// 
	// Process input from the confined cursor. 
	// 

 // Restore the cursor to its previous area. 

	//ClipCursor(&rcOldClip);

	//RECT clip ={400,300,500,400};
	//ClipCursor(&clip);
	//ClipCursor(&rcOldClip);

	//��]���������߂邽�߂ɃJ�[�\���Œ���O��
	//�J�[�\�������͈̔͂ɂƂǂ߂�

	////�L�[�ŃJ��������
	//if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
	//	matRot *= XMMatrixRotationY(0.02f);
	//}
	//else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
	//	matRot *= XMMatrixRotationY(-0.02f);
	//}

	if (Input::GetInstance()->TriggerKey(DIK_C)) {
		if (showCorsl == true) {
			ShowCursor(0);
			showCorsl = false;
		}
		else {
			int num = ShowCursor(1);
			showCorsl = true;
		}
	}

	//�J�����̌����A�ʒu���v�Z
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	//XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rote.y));
	movement = XMVector3TransformNormal(movement, matRot);
	movement *= XMVECTOR{ -1, -1, -1 };
	SetEye(playerObj->GetPosition() + (movement * 30));
	eye.y = 30;
	eye.z -= 10;
	target = playerObj->GetPosition();
	target.y = 10;


	//�v���C���[�̉�]�����ɃJ�����������[�񂷂邽�߂̏���
	if (player != nullptr) {
		//if (player->GetSphere() == true) {
		//	float x = cos(player->object->GetRotation().y * 3.141592 / 180);
		//	float y = sin(player->object->GetRotation().y * 3.141592 / 180);
		//	XMFLOAT3 dir = { -x,0,y };

		//	if (dirEye.x < 5) {
		//		dirEye = dir * 20;
		//		dirTarget = dir * 20;
		//	}
		//}
		//eye = eye + dirEye;
		//target = target + dirTarget;
	}
	

	
}
