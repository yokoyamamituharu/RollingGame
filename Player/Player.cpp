#include "Player.h"
using namespace DirectX;

void Player::Initialize(Input* input, InputMouse* mouse, Camera* camera)
{
	assert(input);
	assert(mouse);
	this->input = input;
	this->mouse = mouse;
	this->camera = camera;
	//プレイヤー用モデルを読み込み
	playermodel = Model::Create("player");
	playerSpheremodel = Model::Create("playerSphere");

	//オブジェクトの作成
	object = OBJobject::Create();
	object->SetModel(playermodel);
}

void Player::Update()
{
	if (Input::GetInstance()->PushKey(DIK_3)) {
		object->SetRotation({
			object->GetRotation().x,
			object->GetRotation().y ,
			object->GetRotation().z + 1.0f });
	}

	if (Input::GetInstance()->TriggerKey(DIK_9)) {
		sceneType *= -1;
	}


	Move();
	//オブジェクトのアップデート
	object->Update();
}

void Player::Move()
{
	DirectX::XMVECTOR forvardvec = {};

	if (sphereFlag == false) {
		if (Input::GetInstance()->PushKey(DIK_W)) {
			forvardvec.m128_f32[2] += 1;
		}
		if (Input::GetInstance()->PushKey(DIK_S)) {
			forvardvec.m128_f32[2] -= 1;
		}
		if (Input::GetInstance()->PushKey(DIK_A)) {
			forvardvec.m128_f32[0] -= 1;
		}
		if (Input::GetInstance()->PushKey(DIK_D)) {
			forvardvec.m128_f32[0] += 1;
		}
	}



#pragma region 回転移動
	//回転移動
	if (mouse->PushMouse(MouseDIK::M_LEFT)) {
		isSphere = true;
	}
	else {
		isSphere = false;
	}


	if (sceneType == 1)
	{
		if (isSphere) {
			//モデルを変える
			object->SetModel(playerSpheremodel);
			//マウスの下への移動量を保存（下に下げれば＋、上にあげれば―（0以下にはならない））
			rollingSpeed += mouse->MoveMouseVector('y') / 30;
			if (rollingSpeed < 0) {
				rollingSpeed = 0;
			}
			//マウスの移動量をプレイヤーの回転速度にもする
			spiralSpeed.z = rollingSpeed;
			//マウスを離したとき、移動量があったらプレイヤーを直進させる

			//その時のプレイヤーの回転速度はプレイヤーの移動速度に依存

			sphereFlag = true;
		}
		else if (rollingSpeed > 0) {
			if (rollingSpeed > 400) {
				forvardvec.m128_f32[2] += 10;
			}
			else if (rollingSpeed > 300) {
				forvardvec.m128_f32[2] += 7;
			}
			else if (rollingSpeed > 100) {
				forvardvec.m128_f32[2] += 5;
			}
			else {
				forvardvec.m128_f32[2] += 3;
			}
			rollingSpeed -= 1;
			attackFlag = true;
		}
		else {
			rollingSpeed = 0.0f;
			spiralSpeed.z = 0;
			object->SetRotation({ object->GetRotation().x, object->GetRotation().y, 0.0f, });
			object->SetModel(playermodel);
			attackFlag = false;
			sphereFlag = false;
		}
		SpiralVector(spiralSpeed);
	}
	else
	{
		if (isSphere) {
			//モデルを変える
			object->SetModel(playerSpheremodel);
			//マウスの下への移動量を保存（下に下げれば＋、上にあげれば―（0以下にはならない））
			rollingSpeed += mouse->MoveMouseVector('y') / 30;
			if (rollingSpeed < 0) {
				rollingSpeed = 0;
			}
			//マウスの移動量をプレイヤーの回転速度にもする
			spiralSpeed.z = rollingSpeed;
			//マウスを離したとき、移動量があったらプレイヤーを直進させる

			//その時のプレイヤーの回転速度はプレイヤーの移動速度に依存

			sphereFlag = true;
		}
		else if (rollingSpeed > 0) {
			rollingSpeed = 10;
			forvardvec.m128_f32[2] += rollingSpeed * 0.5;
			attackFlag = true;
			rollingTime++;
			if (rollingTime > 30) {
				rollingSpeed = 0;
				rollingTime = 0;
			}
		}
		else {
			rollingSpeed = 0.0f;
			spiralSpeed.z = 0;
			object->SetRotation({ object->GetRotation().x, object->GetRotation().y, 0.0f, });
			object->SetModel(playermodel);
			attackFlag = false;
			sphereFlag = false;
		}
		SpiralVector(spiralSpeed);
	}
#pragma endregion

	//これは進む方向にプレイヤーを向かせる処理
	////移動の反映
	//XMVECTOR playermatrot = { forvardvec };
	////回転行列をかける
	//playermatrot = XMVector3Normalize(playermatrot);
	//playermatrot = XMVector3Transform(playermatrot, camera->matRot);
	////正規化する
	//playermatrot = XMVector3Normalize(playermatrot);

	forvardvec = XMVector3TransformNormal(forvardvec, camera->matRot);
	//forvardvec = XMVector3TransformNormal(forvardvec, object->GetMatRot());
	float speed = 1.2f;
	XMFLOAT3 move = { forvardvec.m128_f32[0] * speed,forvardvec.m128_f32[1] * speed,forvardvec.m128_f32[2] * speed };
	object->SetPosition({
		object->GetPosition().x + move.x,
		object->GetPosition().y + move.y,
		object->GetPosition().z + move.z });

	//プレイヤーを真正面に向かせる
	//float buff = atan2f(playermatrot.m128_f32[0], playermatrot.m128_f32[2]);
	//object->SetRotation(XMFLOAT3(0, buff * 180.0f / 3.14f, 0));
}

void Player::Res(int num)
{
	if (num == 1 && resFlag2 == false) {
		resFlag2 = true;
	}

	if (resFlag2 == true) {
		XMVECTOR movement = { 0, 0, 1.0f, 0 };
		XMMATRIX matRot = XMMatrixRotationY((XMConvertToRadians(object->GetRotation().y - 90.0f)));
		movement = XMVector3TransformNormal(movement, matRot);

		movement *= XMVECTOR{ -1, -1, -1 };
		matRot = XMMatrixRotationY((XMConvertToRadians(object->GetRotation().y)));
		//XMVECTOR oldBossPos;
		XMVECTOR playerFront = /*oldBossPos +*/ movement * XMVECTOR{ 50, 50, 50 };

		rollingSpeed = 0;
		XMFLOAT3 pos1 = object->GetPosition();
		if (resFlag1 == false) {
			object->SetPosition({ pos1.x,10, pos1.z });
			resFlag1 = true;
		}
		pos1 = object->GetPosition();
		pos1.y -= 0.1f;
		object->SetPosition({ pos1.x,pos1.y, pos1.z });
		if (pos1.y < -6) {
			resFlag1 = false;
			resFlag2 = false;
		}
		resTimer++;
	}
}
