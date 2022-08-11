#include "Enemy.h"
using namespace DirectX;

void Enemy::Initialize(Input* input, InputMouse* mouse, Camera* camera)
{
	assert(input);
	assert(mouse);
	this->input = input;
	this->mouse = mouse;
	this->camera = camera;
	//プレイヤー用モデルを読み込み
	playermodel = new Model();
	playermodel->CreateFromOBJ("enemy");
	playerSpheremodel = new Model();
	playerSpheremodel->CreateFromOBJ("playerSphere");

	//オブジェクトの作成
	object = OBJobject::Create();
	object->SetModel(playermodel);
}

void Enemy::Update()
{
	//Move();
	//オブジェクトのアップデート
	object->Update();
}

void Enemy::Move()
{
	DirectX::XMVECTOR forvardvec = {};
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


#pragma region 回転移動
	//回転移動
	if (mouse->PushMouse(MouseDIK::M_LEFT)) {
		isSphere = true;
	}
	else {
		isSphere = false;
	}


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

	}
	else if (rollingSpeed > 0) {
		forvardvec.m128_f32[2] += rollingSpeed * 0.5;
		rollingSpeed -= 1;
		attackFlag = true;
	}
	else {
		rollingSpeed = 0.0f;
		spiralSpeed.z = 0;
		object->SetRotation({ object->GetRotation().x, object->GetRotation().y, 0.0f, });
		object->SetModel(playermodel);
		attackFlag = false;
	}
	SpiralVector(spiralSpeed);
#pragma endregion


	//移動の反映
	XMVECTOR playermatrot = { forvardvec };
	//回転行列をかける
	playermatrot = XMVector3Normalize(playermatrot);
	playermatrot = XMVector3Transform(playermatrot, camera->matRot);
	//正規化する
	playermatrot = XMVector3Normalize(playermatrot);

	forvardvec = XMVector3TransformNormal(forvardvec, camera->matRot);
	float speed = 1.0f;
	XMFLOAT3 move = { forvardvec.m128_f32[0] * speed,forvardvec.m128_f32[1] * speed,forvardvec.m128_f32[2] * speed };
	object->SetPosition({
		object->GetPosition().x + move.x,
		object->GetPosition().y + move.y,
		object->GetPosition().z + move.z });

	//プレイヤーを真正面に向かせる
	//float buff = atan2f(playermatrot.m128_f32[0], playermatrot.m128_f32[2]);
	//object->SetRotation(XMFLOAT3(0, buff * 180.0f / 3.14f, 0));
}
