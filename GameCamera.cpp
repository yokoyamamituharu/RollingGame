#include "GameCamera.h"
#include "Input.h"
#include "InputMouse.h"

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
	//マウスでカメラ操作するかの切り替え操作
	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
		if (isSetMousePoint == true)  isSetMousePoint = false;
		else  isSetMousePoint = true;
	}
	//マウスでカメラ操作//ウィンドウがアクティブ状態なら処理
	if (WinApp::GetInstance()->GetHwnd() == GetActiveWindow() && isSetMousePoint == true) {
		matRot *= XMMatrixRotationY(0.8f * InputMouse::GetInstance()->MoveMouseVector('x') / 1000);
		InputMouse::GetInstance()->SetCenterCoursolPos();
	}
	//キーでカメラ操作
	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		matRot *= XMMatrixRotationY(0.02f);
	}
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		matRot *= XMMatrixRotationY(-0.02f);
	}

	//カメラの向き、位置を計算
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	//XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rote.y));
	movement = XMVector3TransformNormal(movement, matRot);
	movement *= XMVECTOR{ -1, -1, -1 };
	SetEye(playerObj->GetPosition() + (movement * 80));
	eye.y = 20;
	target = playerObj->GetPosition();


	//ビュー行列の更新
	UpdateViewMatrix();
	// ビュープロジェクションの合成
	matViewProjection = matView * matProjection;
}
