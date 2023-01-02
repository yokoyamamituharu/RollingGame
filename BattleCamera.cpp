#include "BattleCamera.h"
#include "Input.h"
#include "InputMouse.h"
#include "Useful.h"

BattleCamera* BattleCamera::Create()
{
	//カメラクラスのインスタンスを生成
	BattleCamera* ins = new BattleCamera;
	//初期化処理
	ins->Initialize();
	//生成したインスタンスを返す
	return ins;
}

void BattleCamera::Update()
{
	eye = eye - dirEye;
	target = target - dirTarget;
	dirEye = {0.0f,0.0f ,0.0f };
	dirTarget = { 0.0f,0.0f ,0.0f };

	assert(playerObj);
	//マウスでカメラ操作するかの切り替え操作
	if (Input::GetInstance()->TriggerKey(DIK_F2)) {
		if (isSetMousePoint == true)  isSetMousePoint = false;
		else  isSetMousePoint = true;
	}
	//マウスでカメラ操作//ウィンドウがアクティブ状態なら処理
	if (WinApp::GetInstance()->GetHwnd() == GetActiveWindow() && isSetMousePoint == true) {
		//matRot *= XMMatrixRotationY(0.8f * InputMouse::GetInstance()->MoveMouseVector('x') / 1000);
		InputMouse::GetInstance()->SetCenterCoursolPos();
	}
	////キーでカメラ操作
	//if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
	//	matRot *= XMMatrixRotationY(0.02f);
	//}
	//else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
	//	matRot *= XMMatrixRotationY(-0.02f);
	//}

	if (Input::GetInstance()->TriggerKey(DIK_C)) {
		if (showCorsl == true) {
			showCorsl = false;
		}
		else {
			showCorsl = true;
		}
	}
	showCorsl = true;
	ShowCursor(showCorsl);

	//カメラの向き、位置を計算
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	//XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rote.y));
	movement = XMVector3TransformNormal(movement, matRot);
	movement *= XMVECTOR{ -1, -1, -1 };
	SetEye(playerObj->GetPosition() + (movement * 30));
	eye.y = 30;
	eye.z -= 10;
	target = playerObj->GetPosition();
	target.y = 10;


	if (player != nullptr) {
		if (player->GetSphere() == true) {
			float x = cos(player->object->GetRotation().y * 3.141592 / 180);
			float y = sin(player->object->GetRotation().y * 3.141592 / 180);
			XMFLOAT3 dir = { -x,0,y };

			if (dirEye.x < 5) {
				dirEye = dir * 20;
				dirTarget = dir * 20;
			}
		}
	}

	eye = eye + dirEye;
	target = target + dirTarget;

	//ビュー行列の更新
	UpdateViewMatrix();
	// ビュープロジェクションの合成
	matViewProjection = matView * matProjection;
}
