#include "Player.h"
#include "Useful.h"
#include "ModelManager.h"
#include "safe_delete.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"

using namespace DirectX;

int Player::breakEnemy = 0;
int Player::maxHp = 5;
int Player::hp = Player::maxHp;

Player::Player()
{
}

Player::~Player()
{
	safe_delete(object);
}

Player* Player::Create(Camera* camera, int InOrOut)
{
	Player* ins = new Player;
	ins->Initialize(camera, InOrOut);
	return ins;
}

void Player::Initialize(Camera* camera, int InOrOut)
{
	assert(camera);
	this->camera = camera;
	//プレイヤー用モデルを読み込み
	playermodel = ModelManager::GetModel("player");
	playerSpheremodel = ModelManager::GetModel("playerSphere");

	//オブジェクトの作成
	object = ObjectObj::Create();
	object->SetModel(playermodel);
	object->SetPosition({ 0.0f,-6.0f,-50.0f });
	object->SetRotation({ 0.0f,90.0f,0.0f });
	if (InOrOut == 1) {
		object->SetCollider(new SphereCollider({ 0,0,0 }, 6.0f));
		object->collider->SetAttribute(COLLISION_ATTR_ALLIES);
	}

	shadowObj = ObjectObj::Create(ModelManager::GetModel("shadow"));
	shadowObj->SetScale({ 5,1,5 });

	for (int i = 0; i < 4; i++) {
		wave[i] = ObjectObj::Create(ModelManager::GetModel("wave"));
		wave[i]->SetScale({10,10,10});
	}

	breakEnemy = 0;
}

void Player::Update()
{
	//外シーンではY座標をとりあえず固定
	if (object->collider) {
		object->SetPosY(-6.0f);
	}
	//if (Input::GetInstance()->PushKey(DIK_3)) {
	//	object->SetRotation({
	//		object->GetRotation().x,
	//		object->GetRotation().y ,
	//		object->GetRotation().z + 1.0f });
	//}

	if (muteki == true) {
		mutekiTime++;
		if (mutekiTime > 60) {
			muteki = false;
			mutekiTime = 0;
		}
	}


	object->SetPosition({
		object->GetPosition().x + move.x,
		object->GetPosition().y + move.y,
		object->GetPosition().z + move.z });
	//Move();
	Res();


	//オブジェクトのアップデート
	object->Update();

	object->UpdateWorldMatrix();
	if (object->collider) {
		object->collider->Update();

		SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(object->collider);
		assert(sphereCollider);

		// クエリーコールバッククラス
		class PlayerQueryCallback : public QueryCallback
		{
		public:
			PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

			// 衝突時コールバック関数
			bool OnQueryHit(const QueryHit& info) {

				const XMVECTOR up = { 0,1,0,0 };

				XMVECTOR rejectDir = XMVector3Normalize(info.reject);
				float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

				// 地面判定しきい値
				const float threshold = cosf(XMConvertToRadians(30.0f));

				if (-threshold < cos && cos < threshold) {
					sphere->center += info.reject;
					move += info.reject;
				}

				return true;
			}

			Sphere* sphere = nullptr;
			DirectX::XMVECTOR move = {};
		};

		PlayerQueryCallback callback(sphereCollider);

		XMFLOAT3 position = object->GetPosition();

		// 球と地形の交差を全検索
		CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
		// 交差による排斥分動かす
		position.x += callback.move.m128_f32[0];
		position.y += callback.move.m128_f32[1];
		position.z += callback.move.m128_f32[2];
		// ワールド行列更新
		object->SetPosition(position);
		object->UpdateWorldMatrix();
		object->collider->Update();

		// 球の上端から球の下端までのレイキャスト
		Ray ray;
		ray.start = sphereCollider->center;
		ray.start.m128_f32[1] += sphereCollider->GetRadius();
		ray.dir = { 0,-1,0,0 };
		RaycastHit raycastHit;

		position = object->GetPosition();
		// 接地状態
		//if (onGround) {
		//	// スムーズに坂を下る為の吸着距離
		//	const float adsDistance = 0.2f;
		//	// 接地を維持
		//	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
		//		onGround = true;
		//		position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		//	}
		//	// 地面がないので落下
		//	else {
		//		onGround = false;
		//		fallV = {};
		//	}
		//}
		//// 落下状態
		//else if (fallV.m128_f32[1] <= 0.0f) {
		//	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f)) {
		//		// 着地
		//		onGround = true;
		//		position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		//	}
		//}
		object->SetPosition(position);
	}
	// 行列の更新など
	object->Update();
	shadowObj->SetPosition(object->GetPosition());
	shadowObj->SetPosY(grundHeight - 4);
	shadowObj->Update();
}

void Player::UpdateOut(Camera* camera)
{

	//外シーンではY座標をとりあえず固定
	if (object->collider) {
		object->SetPosY(-6.0f);
	}
	//if (Input::GetInstance()->PushKey(DIK_3)) {
	//	object->SetRotation({
	//		object->GetRotation().x,
	//		object->GetRotation().y ,
	//		object->GetRotation().z + 1.0f });
	//}

	if (muteki == true) {
		mutekiTime++;
		if (mutekiTime > 60) {
			muteki = false;
			mutekiTime = 0;
		}
	}


	object->SetPosition({
		object->GetPosition().x + move.x,
		object->GetPosition().y + move.y,
		object->GetPosition().z + move.z });
	//Move();
	Res();

	camera->UpdateMat();
	//オブジェクトのアップデート
	object->Update();

	object->UpdateWorldMatrix();
	if (object->collider) {
		object->collider->Update();

		SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(object->collider);
		assert(sphereCollider);	

		// クエリーコールバッククラス
		class PlayerQueryCallback : public QueryCallback
		{
		public:
			PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

			// 衝突時コールバック関数
			bool OnQueryHit(const QueryHit& info) {

				const XMVECTOR up = { 0,1,0,0 };

				XMVECTOR rejectDir = XMVector3Normalize(info.reject);
				float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

				// 地面判定しきい値
				const float threshold = cosf(XMConvertToRadians(30.0f));

				if (-threshold < cos && cos < threshold) {
					sphere->center += info.reject;
					move += info.reject;
				}

				return true;
			}

			Sphere* sphere = nullptr;
			DirectX::XMVECTOR move = {};
		};

		PlayerQueryCallback callback(sphereCollider);

		XMFLOAT3 position = object->GetPosition();

		// 球と地形の交差を全検索
		CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
		// 交差による排斥分動かす
		position.x += callback.move.m128_f32[0];
		position.y += callback.move.m128_f32[1];
		position.z += callback.move.m128_f32[2];
		// ワールド行列更新
		object->SetPosition(position);
		object->UpdateWorldMatrix();
		object->collider->Update();

		// 球の上端から球の下端までのレイキャスト
		Ray ray;
		ray.start = sphereCollider->center;
		ray.start.m128_f32[1] += sphereCollider->GetRadius();
		ray.dir = { 0,-1,0,0 };
		RaycastHit raycastHit;

		position = object->GetPosition();
		// 接地状態
		//if (onGround) {
		//	// スムーズに坂を下る為の吸着距離
		//	const float adsDistance = 0.2f;
		//	// 接地を維持
		//	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
		//		onGround = true;
		//		position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		//	}
		//	// 地面がないので落下
		//	else {
		//		onGround = false;
		//		fallV = {};
		//	}
		//}
		//// 落下状態
		//else if (fallV.m128_f32[1] <= 0.0f) {
		//	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f)) {
		//		// 着地
		//		onGround = true;
		//		position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		//	}
		//}
		object->SetPosition(position);
	}
	// 行列の更新など
	object->Update();
	shadowObj->SetPosition(object->GetPosition());
	shadowObj->SetPosY(grundHeight - 4);
	shadowObj->Update();
}

void Player::UpdateIn()
{
	if (muteki == true) {
		mutekiTime++;
		if (mutekiTime > 60) {
			muteki = false;
			mutekiTime = 0;
		}
	}

	object->SetPosition({
		object->GetPosition().x + move.x,
		object->GetPosition().y + move.y,
		object->GetPosition().z + move.z });
	//Move();
	Res();

	//オブジェクトのアップデート
	object->Update();

	//影の更新
	shadowObj->SetPosition(object->GetPosition());
	shadowObj->SetPosY(grundHeight - 4);
	shadowObj->Update();

	//コライダーを持っていなかったら処理をしない
	if (!object->collider) {
		return;
	}
	object->UpdateWorldMatrix();
	object->collider->Update();

	for (int i = 0; i < 4; i++) {
		wave[i]->SetPosition(object->GetPosition());
		wave[i]->Update();
	}

	SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(object->collider);
	assert(sphereCollider);

	// クエリーコールバッククラス
	class PlayerQueryCallback : public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		// 衝突時コールバック関数
		bool OnQueryHit(const QueryHit& info) {

			const XMVECTOR up = { 0,1,0,0 };

			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			// 地面判定しきい値
			const float threshold = cosf(XMConvertToRadians(30.0f));

			if (-threshold < cos && cos < threshold) {
				sphere->center += info.reject;
				move += info.reject;
			}

			return true;
		}

		Sphere* sphere = nullptr;
		DirectX::XMVECTOR move = {};
	};

	PlayerQueryCallback callback(sphereCollider);

	XMFLOAT3 position = object->GetPosition();

	// 球と地形の交差を全検索
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	// 交差による排斥分動かす
	position.x += callback.move.m128_f32[0];
	position.y += callback.move.m128_f32[1];
	position.z += callback.move.m128_f32[2];
	// ワールド行列更新
	object->SetPosition(position);
	object->UpdateWorldMatrix();
	object->collider->Update();

	// 球の上端から球の下端までのレイキャスト
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;

	position = object->GetPosition();
	// 接地状態
	//if (onGround) {
	//	// スムーズに坂を下る為の吸着距離
	//	const float adsDistance = 0.2f;
	//	// 接地を維持
	//	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
	//		onGround = true;
	//		position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
	//	}
	//	// 地面がないので落下
	//	else {
	//		onGround = false;
	//		fallV = {};
	//	}
	//}
	//// 落下状態
	//else if (fallV.m128_f32[1] <= 0.0f) {
	//	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f)) {
	//		// 着地
	//		onGround = true;
	//		position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
	//	}
	//}
	object->SetPosition(position);
	// 行列の更新など
	object->Update();
	shadowObj->SetPosition(object->GetPosition());
	shadowObj->SetPosY(grundHeight - 4);
	shadowObj->Update();
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
	if (InputMouse::GetInstance()->PushMouse(MouseDIK::M_LEFT)) {
		isSphere = true;
	}
	else {
		isSphere = false;
	}

	if (isSphere) {
		//モデルを変える
		object->SetModel(playerSpheremodel);
		//マウスの下への移動量を保存（下に下げれば＋、上にあげれば―（0以下にはならない））
		rollingSpeed += InputMouse::GetInstance()->MoveMouseVector('y') / 30;
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
#pragma region カクス
	//if (sceneType == 1)
	//{
	//}
	//else
	//{
	//	if (isSphere) {
	//		//モデルを変える
	//		object->SetModel(playerSpheremodel);
	//		//マウスの下への移動量を保存（下に下げれば＋、上にあげれば―（0以下にはならない））
	//		rollingSpeed += mouse->MoveMouseVector('y') / 30;
	//		if (rollingSpeed < 0) {
	//			rollingSpeed = 0;
	//		}
	//		//マウスの移動量をプレイヤーの回転速度にもする
	//		spiralSpeed.z = rollingSpeed;
	//		//マウスを離したとき、移動量があったらプレイヤーを直進させる

	//		//その時のプレイヤーの回転速度はプレイヤーの移動速度に依存

	//		sphereFlag = true;
	//	}
	//	else if (rollingSpeed > 0) {
	//		rollingSpeed = 10;
	//		forvardvec.m128_f32[2] += rollingSpeed * 0.5;
	//		attackFlag = true;
	//		rollingTime++;
	//		if (rollingTime > 30) {
	//			rollingSpeed = 0;
	//			rollingTime = 0;
	//		}
	//	}
	//	else {
	//		rollingSpeed = 0.0f;
	//		spiralSpeed.z = 0;
	//		object->SetRotation({ object->GetRotation().x, object->GetRotation().y, 0.0f, });
	//		object->SetModel(playermodel);
	//		attackFlag = false;
	//		sphereFlag = false;
	//	}
	//	SpiralVector(spiralSpeed);
	//}
#pragma endregion


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
	move = { forvardvec.m128_f32[0] * speed,forvardvec.m128_f32[1] * speed,forvardvec.m128_f32[2] * speed };
	//object->SetPosition({
	//	object->GetPosition().x + move.x,
	//	object->GetPosition().y + move.y,
	//	object->GetPosition().z + move.z });

	//プレイヤーを真正面に向かせる
	//float buff = atan2f(playermatrot.m128_f32[0], playermatrot.m128_f32[2]);
	//object->SetRotation(XMFLOAT3(0, buff * 180.0f / 3.14f, 0));
}

void Player::MoveIn()
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
	if (InputMouse::GetInstance()->TorigerMouse(MouseDIK::M_LEFT)) {
		clickTrigerPos = InputMouse::GetInstance()->GetPos();
	}

	if (InputMouse::GetInstance()->PushMouse(MouseDIK::M_LEFT)) {
		isSphere = true;
	}
	else {
		isSphere = false;
	}

	if (InputMouse::GetInstance()->ReleaseMouse(MouseDIK::M_LEFT)) {
		XMFLOAT2 releasePos = InputMouse::GetInstance()->GetPos();
		XMVECTOR pos1, pos2;
		pos1.m128_f32[0] = clickTrigerPos.x;
		pos1.m128_f32[1] = 0.0f;
		pos1.m128_f32[2] = -clickTrigerPos.y;
		pos2.m128_f32[0] = releasePos.x;
		pos2.m128_f32[1] = 0.0;
		pos2.m128_f32[2] = -releasePos.y;
		attackDirection = pos1 - pos2;
		attackDirection = XMVector3Normalize(attackDirection);
		attackDirection.m128_f32[1] = 0;//ここを0にしないとプレイヤーと敵のY座標のずれで敵の突進方向がずれる
	}

	if (isSphere) {
		//モデルを変える
		object->SetModel(playerSpheremodel);
		//マウスの下への移動量を保存（下に下げれば＋、上にあげれば―（0以下にはならない））
		rollingSpeed = 20;
		if (rollingSpeed < 0) {
			rollingSpeed = 0;
		}
		//マウスの移動量をプレイヤーの回転速度にもする
		spiralSpeed.z = 100;
		//マウスを離したとき、移動量があったらプレイヤーを直進させる

		//その時のプレイヤーの回転速度はプレイヤーの移動速度に依存

		sphereFlag = true;
	}
	else if (rollingSpeed > 0) {
		//forvardvec.m128_f32[2] += 6.5;
		XMVECTOR pos = XMLoadFloat3(&object->GetPosition());
		pos += attackDirection * 6.0f;
		object->SetPosition(Use::LoadXMVECTOR(pos));
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
	move = { forvardvec.m128_f32[0] * speed,forvardvec.m128_f32[1] * speed,forvardvec.m128_f32[2] * speed };
	//object->SetPosition({
	//	object->GetPosition().x + move.x,
	//	object->GetPosition().y + move.y,
	//	object->GetPosition().z + move.z });

	//プレイヤーを真正面に向かせる
	//float buff = atan2f(playermatrot.m128_f32[0], playermatrot.m128_f32[2]);
	//object->SetRotation(XMFLOAT3(0, buff * 180.0f / 3.14f, 0));
}

void Player::Res(bool flag, XMFLOAT3 vec)
{
	object->VecSetPosition(backVec);

	//下降処理
	if (resFlag2 == true) {
		if (gravity <= 2.0f) {
			gravity += 0.03f;
		}
		object->SetPosY({ object->GetPosition().y - gravity });
		if (object->GetPosition().y <= grundHeight) {
			resFlag2 = false;
			gravity = 0.0f;
			object->SetPosY(-6.0f);
			backVec = { 0,0,0 };
		}
	}

	//上昇処理
	if (resFlag1 == true) {
		timer += 0.0625f;
		float un = Ease(timer, 1.6);
		object->SetPosY({ posY + riseValue * un });
		if (timer >= 1) {
			resFlag1 = false;
			resFlag2 = true;
		}
	}

	//バウンドを起動
	if (flag == 1 && resFlag1 == false) {
		posY = object->GetPosition().y;
		resFlag1 = true;
		timer = 0;
		rollingSpeed = 0;
		backVec = Use::LoadXMVECTOR(-XMVector3Normalize(XMLoadFloat3(&move)));
	}
}

void Player::Stop()
{
	rollingSpeed = 0;
	object->SetPosition(outPos);
	resFlag1 = false;
	resFlag2 = false;
	gravity = 0.0f;
	backVec = { 0,0,0 };
}

void Player::Draw()
{
	if (rollingSpeed > 0) {
		wave[0]->Draw();
	}
	if (muteki == true) {
		if (mutekiTime % 2 == 0) {
			object->Draw();
		}
	}
	else {
		object->Draw();
	}
	shadowObj->Draw();
}

float Player::Ease(float x, float s)
{
	if (x < 1.0f) {
		double num = pow(1 - x, s);
		double num2 = 1 - num;
		return num2;
	}
	else
	{
		//xが1を超えた値だとpowの返り値がおかしくなるので
		return 1.0f;
	}
}

void Player::Damage(int damage)
{
	if (muteki == false) {
		hp -= damage;
		muteki = true;
	}
}

void Player::StopRolling()
{
	rollingSpeed = 0;
	rollingTime = 0;
}


