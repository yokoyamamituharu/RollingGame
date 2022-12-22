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
	//�v���C���[�p���f����ǂݍ���
	playermodel = ModelManager::GetModel("player");
	playerSpheremodel = ModelManager::GetModel("playerSphere");

	//�I�u�W�F�N�g�̍쐬
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
	//�O�V�[���ł�Y���W���Ƃ肠�����Œ�
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


	//�I�u�W�F�N�g�̃A�b�v�f�[�g
	object->Update();

	object->UpdateWorldMatrix();
	if (object->collider) {
		object->collider->Update();

		SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(object->collider);
		assert(sphereCollider);

		// �N�G���[�R�[���o�b�N�N���X
		class PlayerQueryCallback : public QueryCallback
		{
		public:
			PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

			// �Փˎ��R�[���o�b�N�֐�
			bool OnQueryHit(const QueryHit& info) {

				const XMVECTOR up = { 0,1,0,0 };

				XMVECTOR rejectDir = XMVector3Normalize(info.reject);
				float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

				// �n�ʔ��肵�����l
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

		// ���ƒn�`�̌�����S����
		CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
		// �����ɂ��r�˕�������
		position.x += callback.move.m128_f32[0];
		position.y += callback.move.m128_f32[1];
		position.z += callback.move.m128_f32[2];
		// ���[���h�s��X�V
		object->SetPosition(position);
		object->UpdateWorldMatrix();
		object->collider->Update();

		// ���̏�[���狅�̉��[�܂ł̃��C�L���X�g
		Ray ray;
		ray.start = sphereCollider->center;
		ray.start.m128_f32[1] += sphereCollider->GetRadius();
		ray.dir = { 0,-1,0,0 };
		RaycastHit raycastHit;

		position = object->GetPosition();
		// �ڒn���
		//if (onGround) {
		//	// �X���[�Y�ɍ������ׂ̋z������
		//	const float adsDistance = 0.2f;
		//	// �ڒn���ێ�
		//	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
		//		onGround = true;
		//		position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		//	}
		//	// �n�ʂ��Ȃ��̂ŗ���
		//	else {
		//		onGround = false;
		//		fallV = {};
		//	}
		//}
		//// �������
		//else if (fallV.m128_f32[1] <= 0.0f) {
		//	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f)) {
		//		// ���n
		//		onGround = true;
		//		position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		//	}
		//}
		object->SetPosition(position);
	}
	// �s��̍X�V�Ȃ�
	object->Update();
	shadowObj->SetPosition(object->GetPosition());
	shadowObj->SetPosY(grundHeight - 4);
	shadowObj->Update();
}

void Player::UpdateOut(Camera* camera)
{

	//�O�V�[���ł�Y���W���Ƃ肠�����Œ�
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
	//�I�u�W�F�N�g�̃A�b�v�f�[�g
	object->Update();

	object->UpdateWorldMatrix();
	if (object->collider) {
		object->collider->Update();

		SphereCollider* sphereCollider = dynamic_cast<SphereCollider*>(object->collider);
		assert(sphereCollider);	

		// �N�G���[�R�[���o�b�N�N���X
		class PlayerQueryCallback : public QueryCallback
		{
		public:
			PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

			// �Փˎ��R�[���o�b�N�֐�
			bool OnQueryHit(const QueryHit& info) {

				const XMVECTOR up = { 0,1,0,0 };

				XMVECTOR rejectDir = XMVector3Normalize(info.reject);
				float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

				// �n�ʔ��肵�����l
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

		// ���ƒn�`�̌�����S����
		CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
		// �����ɂ��r�˕�������
		position.x += callback.move.m128_f32[0];
		position.y += callback.move.m128_f32[1];
		position.z += callback.move.m128_f32[2];
		// ���[���h�s��X�V
		object->SetPosition(position);
		object->UpdateWorldMatrix();
		object->collider->Update();

		// ���̏�[���狅�̉��[�܂ł̃��C�L���X�g
		Ray ray;
		ray.start = sphereCollider->center;
		ray.start.m128_f32[1] += sphereCollider->GetRadius();
		ray.dir = { 0,-1,0,0 };
		RaycastHit raycastHit;

		position = object->GetPosition();
		// �ڒn���
		//if (onGround) {
		//	// �X���[�Y�ɍ������ׂ̋z������
		//	const float adsDistance = 0.2f;
		//	// �ڒn���ێ�
		//	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
		//		onGround = true;
		//		position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		//	}
		//	// �n�ʂ��Ȃ��̂ŗ���
		//	else {
		//		onGround = false;
		//		fallV = {};
		//	}
		//}
		//// �������
		//else if (fallV.m128_f32[1] <= 0.0f) {
		//	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f)) {
		//		// ���n
		//		onGround = true;
		//		position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
		//	}
		//}
		object->SetPosition(position);
	}
	// �s��̍X�V�Ȃ�
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

	//�I�u�W�F�N�g�̃A�b�v�f�[�g
	object->Update();

	//�e�̍X�V
	shadowObj->SetPosition(object->GetPosition());
	shadowObj->SetPosY(grundHeight - 4);
	shadowObj->Update();

	//�R���C�_�[�������Ă��Ȃ������珈�������Ȃ�
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

	// �N�G���[�R�[���o�b�N�N���X
	class PlayerQueryCallback : public QueryCallback
	{
	public:
		PlayerQueryCallback(Sphere* sphere) : sphere(sphere) {};

		// �Փˎ��R�[���o�b�N�֐�
		bool OnQueryHit(const QueryHit& info) {

			const XMVECTOR up = { 0,1,0,0 };

			XMVECTOR rejectDir = XMVector3Normalize(info.reject);
			float cos = XMVector3Dot(rejectDir, up).m128_f32[0];

			// �n�ʔ��肵�����l
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

	// ���ƒn�`�̌�����S����
	CollisionManager::GetInstance()->QuerySphere(*sphereCollider, &callback, COLLISION_ATTR_LANDSHAPE);
	// �����ɂ��r�˕�������
	position.x += callback.move.m128_f32[0];
	position.y += callback.move.m128_f32[1];
	position.z += callback.move.m128_f32[2];
	// ���[���h�s��X�V
	object->SetPosition(position);
	object->UpdateWorldMatrix();
	object->collider->Update();

	// ���̏�[���狅�̉��[�܂ł̃��C�L���X�g
	Ray ray;
	ray.start = sphereCollider->center;
	ray.start.m128_f32[1] += sphereCollider->GetRadius();
	ray.dir = { 0,-1,0,0 };
	RaycastHit raycastHit;

	position = object->GetPosition();
	// �ڒn���
	//if (onGround) {
	//	// �X���[�Y�ɍ������ׂ̋z������
	//	const float adsDistance = 0.2f;
	//	// �ڒn���ێ�
	//	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f + adsDistance)) {
	//		onGround = true;
	//		position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
	//	}
	//	// �n�ʂ��Ȃ��̂ŗ���
	//	else {
	//		onGround = false;
	//		fallV = {};
	//	}
	//}
	//// �������
	//else if (fallV.m128_f32[1] <= 0.0f) {
	//	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, sphereCollider->GetRadius() * 2.0f)) {
	//		// ���n
	//		onGround = true;
	//		position.y -= (raycastHit.distance - sphereCollider->GetRadius() * 2.0f);
	//	}
	//}
	object->SetPosition(position);
	// �s��̍X�V�Ȃ�
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

#pragma region ��]�ړ�
	//��]�ړ�
	if (InputMouse::GetInstance()->PushMouse(MouseDIK::M_LEFT)) {
		isSphere = true;
	}
	else {
		isSphere = false;
	}

	if (isSphere) {
		//���f����ς���
		object->SetModel(playerSpheremodel);
		//�}�E�X�̉��ւ̈ړ��ʂ�ۑ��i���ɉ�����΁{�A��ɂ�����΁\�i0�ȉ��ɂ͂Ȃ�Ȃ��j�j
		rollingSpeed += InputMouse::GetInstance()->MoveMouseVector('y') / 30;
		if (rollingSpeed < 0) {
			rollingSpeed = 0;
		}
		//�}�E�X�̈ړ��ʂ��v���C���[�̉�]���x�ɂ�����
		spiralSpeed.z = rollingSpeed;
		//�}�E�X�𗣂����Ƃ��A�ړ��ʂ���������v���C���[�𒼐i������

		//���̎��̃v���C���[�̉�]���x�̓v���C���[�̈ړ����x�Ɉˑ�

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
#pragma region �J�N�X
	//if (sceneType == 1)
	//{
	//}
	//else
	//{
	//	if (isSphere) {
	//		//���f����ς���
	//		object->SetModel(playerSpheremodel);
	//		//�}�E�X�̉��ւ̈ړ��ʂ�ۑ��i���ɉ�����΁{�A��ɂ�����΁\�i0�ȉ��ɂ͂Ȃ�Ȃ��j�j
	//		rollingSpeed += mouse->MoveMouseVector('y') / 30;
	//		if (rollingSpeed < 0) {
	//			rollingSpeed = 0;
	//		}
	//		//�}�E�X�̈ړ��ʂ��v���C���[�̉�]���x�ɂ�����
	//		spiralSpeed.z = rollingSpeed;
	//		//�}�E�X�𗣂����Ƃ��A�ړ��ʂ���������v���C���[�𒼐i������

	//		//���̎��̃v���C���[�̉�]���x�̓v���C���[�̈ړ����x�Ɉˑ�

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

	//����͐i�ޕ����Ƀv���C���[���������鏈��
	////�ړ��̔��f
	//XMVECTOR playermatrot = { forvardvec };
	////��]�s���������
	//playermatrot = XMVector3Normalize(playermatrot);
	//playermatrot = XMVector3Transform(playermatrot, camera->matRot);
	////���K������
	//playermatrot = XMVector3Normalize(playermatrot);

	forvardvec = XMVector3TransformNormal(forvardvec, camera->matRot);
	//forvardvec = XMVector3TransformNormal(forvardvec, object->GetMatRot());
	float speed = 1.2f;
	move = { forvardvec.m128_f32[0] * speed,forvardvec.m128_f32[1] * speed,forvardvec.m128_f32[2] * speed };
	//object->SetPosition({
	//	object->GetPosition().x + move.x,
	//	object->GetPosition().y + move.y,
	//	object->GetPosition().z + move.z });

	//�v���C���[��^���ʂɌ�������
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

#pragma region ��]�ړ�
	//��]�ړ�
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
		attackDirection.m128_f32[1] = 0;//������0�ɂ��Ȃ��ƃv���C���[�ƓG��Y���W�̂���œG�̓ːi�����������
	}

	if (isSphere) {
		//���f����ς���
		object->SetModel(playerSpheremodel);
		//�}�E�X�̉��ւ̈ړ��ʂ�ۑ��i���ɉ�����΁{�A��ɂ�����΁\�i0�ȉ��ɂ͂Ȃ�Ȃ��j�j
		rollingSpeed = 20;
		if (rollingSpeed < 0) {
			rollingSpeed = 0;
		}
		//�}�E�X�̈ړ��ʂ��v���C���[�̉�]���x�ɂ�����
		spiralSpeed.z = 100;
		//�}�E�X�𗣂����Ƃ��A�ړ��ʂ���������v���C���[�𒼐i������

		//���̎��̃v���C���[�̉�]���x�̓v���C���[�̈ړ����x�Ɉˑ�

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

	//����͐i�ޕ����Ƀv���C���[���������鏈��
	////�ړ��̔��f
	//XMVECTOR playermatrot = { forvardvec };
	////��]�s���������
	//playermatrot = XMVector3Normalize(playermatrot);
	//playermatrot = XMVector3Transform(playermatrot, camera->matRot);
	////���K������
	//playermatrot = XMVector3Normalize(playermatrot);

	forvardvec = XMVector3TransformNormal(forvardvec, camera->matRot);
	//forvardvec = XMVector3TransformNormal(forvardvec, object->GetMatRot());
	float speed = 1.2f;
	move = { forvardvec.m128_f32[0] * speed,forvardvec.m128_f32[1] * speed,forvardvec.m128_f32[2] * speed };
	//object->SetPosition({
	//	object->GetPosition().x + move.x,
	//	object->GetPosition().y + move.y,
	//	object->GetPosition().z + move.z });

	//�v���C���[��^���ʂɌ�������
	//float buff = atan2f(playermatrot.m128_f32[0], playermatrot.m128_f32[2]);
	//object->SetRotation(XMFLOAT3(0, buff * 180.0f / 3.14f, 0));
}

void Player::Res(bool flag, XMFLOAT3 vec)
{
	object->VecSetPosition(backVec);

	//���~����
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

	//�㏸����
	if (resFlag1 == true) {
		timer += 0.0625f;
		float un = Ease(timer, 1.6);
		object->SetPosY({ posY + riseValue * un });
		if (timer >= 1) {
			resFlag1 = false;
			resFlag2 = true;
		}
	}

	//�o�E���h���N��
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
		//x��1�𒴂����l����pow�̕Ԃ�l�����������Ȃ�̂�
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


