#include "GameCamera.h"
#include "Input.h"
#include "InputMouse.h"
#include "Useful.h"

GameCamera* GameCamera::Create()
{
	//カメラクラスのインスタンスを生成
	GameCamera* ins = new GameCamera;
	//初期化処理
	ins->Initialize();
	//生成したインスタンスを返す
	return ins;
}

void GameCamera::Update()
{
	assert(playerObj);
	//マウスでカメラ操作するかの切り替え操作
	if (Input::GetInstance()->TriggerKey(DIK_F2)) {
		isSetMousePoint = !isSetMousePoint;
	}
	if (InputMouse::GetInstance()->PushMouse(MouseDIK::M_LEFT)) {
		isSetMousePoint = false;
	}
	else {
		isSetMousePoint = true;
	}

	//マウスでカメラ操作//ウィンドウがアクティブ状態なら処理
	if (WinApp::GetInstance()->GetHwnd() == GetActiveWindow() && isSetMousePoint == true) {
		matRot *= XMMatrixRotationY(0.8f * InputMouse::GetInstance()->MoveMouseVector().x / 1000);		
		InputMouse::GetInstance()->SetCenterCoursolPos();
	}
	if (flag == true) {
		matRot = XMMatrixIdentity();
	}


	XMFLOAT3 rotation{};
	//キーでカメラ操作
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		rotation.y += 1.0f;
	}
	if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		rotation.y -= 1.0f;
	}
	if (Input::GetInstance()->PushKey(DIK_UP)) {
		rotation.x += 1.0f;
	}
	if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		rotation.x -= 1.0f;
	}

	if (Input::GetInstance()->TriggerKey(DIK_C)) {
		showCorsl = !showCorsl;
		InputMouse::ShowMouseCursor(!showCorsl);
	}

	//カメラの向き、位置を計算
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	//XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rote.y));
	movement = XMVector3TransformNormal(movement, matRot);
	movement *= XMVECTOR{ -1, -1, -1 };
	SetEye(playerObj->GetPosition() + (movement * keisu));
	eye.y += 7;
	target = playerObj->GetPosition();
	target.y += 3;


	//target = eye + (movement * 20);
	//ビュー行列の更新
	UpdateViewMatrix();
	// ビュープロジェクションの合成
	matViewProjection = matView * matProjection;
}
