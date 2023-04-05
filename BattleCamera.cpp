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
	if (InputMouse::GetInstance()->PushMouse(MouseDIK::M_LEFT)) {
		isSetMousePoint = false;
	}
	else {
		isSetMousePoint = true;
	}

	//マウスでカメラ操作//ウィンドウがアクティブ状態なら処理
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

	//回転方向を決めるためにカーソル固定を外す
	//カーソルを一定の範囲にとどめる

	////キーでカメラ操作
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


	//プレイヤーの回転方向にカメラをぐいーんするための処理
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
