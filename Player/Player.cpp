#include "Player.h"
#include "Useful.h"
#include "ModelManager.h"
#include "safe_delete.h"
#include "SphereCollider.h"
#include "CollisionManager.h"
#include "CollisionAttribute.h"
#include "Sprite.h"
#include "Collision.h"

using namespace DirectX;

int Player::breakEnemy = 0;
int Player::maxHp = 5;
int Player::hp = Player::maxHp;
const float Player::groundHeight = 4.0f;

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
	if (InOrOut == 2) {
		object->SetPosition({ 0.0f,-6.0f,-50.0f });
	}
	else {
		object->SetPosition({ 0.0f,groundHeight,-50.0f });
	}

	object->SetRotation({ 0.0f,90.0f,0.0f });
	object->SetScale({ 1.0f,1.0f,1.0f });
	if (InOrOut == 1) {
		object->SetCollider(new SphereCollider({ 0,0,0 }, 6.0f));
		object->collider->SetAttribute(COLLISION_ATTR_ALLIES);
	}

	shadowObj = ObjectObj::Create(ModelManager::GetModel("shadow"));
	shadowObj->SetScale({ 5,1,5 });

	waveNum = sizeof(waveright) / sizeof(waveright[0]);

	for (int i = 0; i < waveNum; i++) {
		waveright[i] = ObjectObj::Create(ModelManager::GetModel("wave"));
		waveright[i]->SetScale({ 7,7,7 });
		waveright[i]->SetRotation({ 0,90,0 });
		waveleft[i] = ObjectObj::Create(ModelManager::GetModel("wave"));
		waveleft[i]->SetScale({ 7,7,7 });
		waveleft[i]->SetRotation({ 0,90,0 });
	}

	breakEnemy = 0;

	yazirusi = Sprite::Create(30, { 0,0 });
	yazirusi->SetAnchorPoint({ 0.5f,0.5f });
}

void Player::UpdateOut(Camera* camera)
{
	//�O�V�[���ł�Y���W���Ƃ肠�����Œ�
	if (object->collider) {
		object->SetPosY(groundHeight);
	}

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
	//MoveOut();
	Res();


	//�������u�Ԃɒ��S�_������
	if (InputMouse::GetInstance()->TorigerMouse(MouseDIK::M_LEFT) && isShoot == false) {
		clickTrigerPos = InputMouse::GetInstance()->GetScreanPos();
	}

	//���S�_����������v�Z���ĕۑ�
	if (InputMouse::GetInstance()->PushMouse(MouseDIK::M_LEFT) && isShoot == false) {
		isSphere = true;
		XMFLOAT2 releasePos = InputMouse::GetInstance()->GetScreanPos();
		XMVECTOR pos1, pos2;
		pos1.m128_f32[0] = clickTrigerPos.x;
		pos1.m128_f32[1] = 0.0f;
		pos1.m128_f32[2] = -clickTrigerPos.y;
		pos2.m128_f32[0] = releasePos.x;
		pos2.m128_f32[1] = 0.0;
		pos2.m128_f32[2] = -releasePos.y;
		attackDirection = pos1 - pos2;
		attackDirection = XMVector3Normalize(attackDirection);
		attackDirection.m128_f32[1] = 0;	//�U������		
	}


	camera->UpdateMat();
	//�I�u�W�F�N�g�̃A�b�v�f�[�g		
	object->Update();
	shadowObj->SetPosition(object->GetPosition());
	shadowObj->SetPosY(groundHeight - 4);
	shadowObj->Update();
	object->UpdateWorldMatrix();

	if (object->collider == nullptr) {
		return;
	}
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
	// �s��̍X�V�Ȃ�
	object->Update();
	shadowObj->SetPosition(object->GetPosition());
	shadowObj->SetPosY(groundHeight - 4);
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

	if (garigariFlag == true && InputMouse::GetInstance()->PushMouse(MouseDIK::M_LEFT)) {
		attackFlag = true;
		attackDirection = { 0,0,0,0 };
	}
	else {
		garigariFlag = false;
	}

	object->SetPosition({
		object->GetPosition().x + move.x,
		object->GetPosition().y + move.y,
		object->GetPosition().z + move.z });
	//Move();
	Res();
	//CrowAttack();
	//�I�u�W�F�N�g�̃A�b�v�f�[�g
	//object->Update();

	//�������u�Ԃɒ��S�_������
	if (InputMouse::GetInstance()->TorigerMouse(MouseDIK::M_LEFT) && isShoot == false) {
		clickTrigerPos = InputMouse::GetInstance()->GetScreanPos();
	}
	//���S�_����������v�Z���ĕۑ�
	if (InputMouse::GetInstance()->PushMouse(MouseDIK::M_LEFT) && isShoot == false) {
		isSphere = true;
		XMFLOAT2 releasePos = InputMouse::GetInstance()->GetScreanPos();
		XMVECTOR pos1, pos2;
		pos1.m128_f32[0] = clickTrigerPos.x;
		pos1.m128_f32[1] = 0.0f;
		pos1.m128_f32[2] = -clickTrigerPos.y;
		pos2.m128_f32[0] = releasePos.x;
		pos2.m128_f32[1] = 0.0;
		pos2.m128_f32[2] = -releasePos.y;
		attackDirection = pos1 - pos2;
		attackDirection = XMVector3Normalize(attackDirection);
		attackDirection.m128_f32[1] = 0;	//�U������

		//�G�t�F�N�g�̌������v�Z
		XMVECTOR ppos1 = XMLoadFloat2(&yazirusi->GetPosition()), ppos2 = XMLoadFloat2(&yazirusi->GetPosition());
		ppos2 += attackDirection * 6.0f;
		const float direction = 180.0f;
		XMFLOAT3 distance = Use::LoadXMVECTOR(ppos1 - ppos2);
		float angleToPlayer = (atan2(distance.x, distance.z)) * 180.0f / 3.14f + direction;
		yazirusi->SetRotation(angleToPlayer);
		if (InputMouse::GetInstance()->MoveMouseVector('y') > 0) {
			yazirusiScale.x += 0.1f;
			yazirusiScale.y += 0.3f;
		}
	}
	if (InputMouse::GetInstance()->ReleaseMouse(MouseDIK::M_LEFT)) {
		yazirusiScale = { 1,1 };
	}
	yazirusi->SetPosition(InputMouse::GetInstance()->GetWindowPos());
	yazirusi->SetScale(yazirusiScale);

	//�e�̍X�V
	shadowObj->SetPosition(object->GetPosition());
	shadowObj->SetPosY(-6.0f - 4);
	shadowObj->Update();

	for (int i = 0; i < waveNum; i++) {
		//wave[i]->SetPosition(object->GetPosition());
		waveright[i]->Update();
		waveleft[i]->Update();
	}
}

void Player::MoveOut()
{
	XMFLOAT3 oldpos = object->GetPosition();
	DirectX::XMVECTOR forvardvec = {};
	if (isSphere == false) {
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
	//forvardvec.m128_f32[2] += 1;
	forvardvec = XMVector3TransformNormal(forvardvec, camera->matRot);
	const float speed = 1.2f;
	move = { forvardvec.m128_f32[0] * speed,forvardvec.m128_f32[1] * speed,forvardvec.m128_f32[2] * speed };

	XMFLOAT3 pos = object->GetPosition() + move;

	//XMFLOAT3 pppos = oldpos - pos;
	XMFLOAT3 pppos = pos - oldpos;
	float yziku =(atan2(pppos.x, pppos.z)) * 180.0f / 3.14f +180;
	//float yziku = atan2(forvardvec.m128_f32[2], forvardvec.m128_f32[1]) * 180.0f / 3.14f + 90.0f;
	object->SetRotation({ object->GetRotation().x,yziku ,object->GetRotation().z });
	RollingMoveOut();
}

void Player::RollingMoveOut()
{
	//��]�ړ�

	//�񋅑̏�Ԃ���Ȃ����ɃN���b�N�����狅�̏��
	if (InputMouse::GetInstance()->PushMouse(MouseDIK::M_LEFT)) {
		isSphere = true;

		XMFLOAT2 windowCenter = { WinApp::window_width / 2,WinApp::window_height / 2 };
		//��]�`���[�W
		if (WinApp::window_height / 2 < InputMouse::GetInstance()->GetWindowPos().y) {
			rollingPower = InputMouse::GetInstance()->GetWindowPos().y - WinApp::window_height / 2;
		}
		else {
			rollingPower = 0;
		}
	}
	else if (!InputMouse::GetInstance()->ReleaseMouse(MouseDIK::M_LEFT) && isShoot == false) {
		isSphere = false;
	}

	if (InputMouse::GetInstance()->ReleaseMouse(MouseDIK::M_LEFT) && rollingPower > 0) {
		isShoot = true;
	}

	if (isShoot == true) {
		XMVECTOR forvardvec = {};
		forvardvec.m128_f32[2] += 3;
		forvardvec = XMVector3TransformNormal(forvardvec, camera->matRot);
		move = move + XMVECTOR{ forvardvec.m128_f32[0], forvardvec.m128_f32[1], forvardvec.m128_f32[2] };

		rollingTime++;
		if (rollingTime >= 120) {
			isShoot = false;
			rollingTime = 0;
			rollingPower = 0;
		}
	}

	if (isSphere || isShoot) {
		object->SetModel(ModelManager::GetModel("playerSphere"));
	}
	else {
		object->SetModel(ModelManager::GetModel("player"));
	}
}

void Player::MoveIn()
{
	if (crawAttackFlag == true) { return; }
	DirectX::XMVECTOR forvardvec = {};

	if (isSphere == false) {
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

	forvardvec = XMVector3TransformNormal(forvardvec, camera->matRot);
	float speed = 1.2f;
	move = { forvardvec.m128_f32[0] * speed,forvardvec.m128_f32[1] * speed,forvardvec.m128_f32[2] * speed };

	RollingMoveIn();
}

void Player::RollingMoveIn()
{
	//�������u�Ԃɒ��S�_������
	if (InputMouse::GetInstance()->TorigerMouse(MouseDIK::M_LEFT) && isShoot == false) {
		clickTrigerPos = InputMouse::GetInstance()->GetScreanPos();
	}

	//���S�_����������v�Z���ĕۑ�
	if (InputMouse::GetInstance()->PushMouse(MouseDIK::M_LEFT) && isShoot == false) {
		isSphere = true;
		XMFLOAT2 releasePos = InputMouse::GetInstance()->GetScreanPos();
		XMVECTOR pos1, pos2;
		pos1.m128_f32[0] = clickTrigerPos.x;
		pos1.m128_f32[1] = 0.0f;
		pos1.m128_f32[2] = -clickTrigerPos.y;
		pos2.m128_f32[0] = releasePos.x;
		pos2.m128_f32[1] = 0.0;
		pos2.m128_f32[2] = -releasePos.y;
		attackDirection = pos1 - pos2;
		attackDirection = XMVector3Normalize(attackDirection);
		attackDirection.m128_f32[1] = 0;	//�U������

		XMVECTOR ppos1 = XMLoadFloat3(&object->GetPosition()), ppos2 = XMLoadFloat3(&object->GetPosition());
		ppos2 += attackDirection * 6.0f;
		const float direction = 270.0f;
		XMFLOAT3 distance = Use::LoadXMVECTOR(ppos1 - ppos2);
		float angleToPlayer = (atan2(distance.x, distance.z)) * 180.0f / 3.14f + direction;
		object->SetRotation(XMFLOAT3(0.0f, angleToPlayer, 0.0f));
	}
	else if (!InputMouse::GetInstance()->PushMouse(MouseDIK::M_LEFT) && isShoot == false) {
		isSphere = false;
	}

	//�N���b�N��������u�Ԃɓːi�������v�Z���ĕۑ�
	if (InputMouse::GetInstance()->ReleaseMouse(MouseDIK::M_LEFT) && isShoot == false) {
		XMFLOAT2 releasePos = InputMouse::GetInstance()->GetScreanPos();
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

		XMVECTOR ppos1 = XMLoadFloat3(&object->GetPosition()), ppos2 = XMLoadFloat3(&object->GetPosition());
		ppos2 += attackDirection * 6.0f;
		const float direction = 270.0f;
		XMFLOAT3 distance = Use::LoadXMVECTOR(ppos1 - ppos2);
		float angleToPlayer = (atan2(distance.x, distance.z)) * 180.0f / 3.14f + direction;
		object->SetRotation(XMFLOAT3(0.0f, angleToPlayer, 0.0f));
		for (int i = 0; i < waveNum; i++) {
			waveright[i]->SetRotation(object->GetRotation());
			waveleft[i]->SetRotation(object->GetRotation());
		}

		isShoot = true;
	}

	if (isShoot == true) {
		XMVECTOR pos = XMLoadFloat3(&object->GetPosition());
		pos += attackDirection * 6.0f;
		attackFlag = true;
		if (roolstop == false) {
			move = Use::LoadXMVECTOR(attackDirection * 6.0f);
		}
		roolTime++;
		if (roolTime > 10) {
			roolTime = 0;
			isShoot = false;
			isSphere = false;
		}
	}
	else if (rollingSpeed > 0) {
		XMVECTOR pos = XMLoadFloat3(&object->GetPosition());
		pos += attackDirection * 6.0f;
		attackFlag = true;
		isShoot = true;
		if (roolstop == false) {
			//object->SetPosition(Use::LoadXMVECTOR(pos));
			move = Use::LoadXMVECTOR(attackDirection * 6.0f);
			rollingSpeed -= 1;
		}
		else {
			if (roolTime >= 10) {
				attackFlag = true;
				roolTime = 0;
			}
			else {
				attackFlag = false;
				roolTime++;
			}
		}
	}
	else {
		rollingSpeed = 0.0f;
		spiralSpeed.z = 0;
		object->SetRotation({ object->GetRotation().x, object->GetRotation().y, 0.0f, });
		object->SetModel(playermodel);
		attackFlag = false;
		isShoot = false;
	}

	if (isSphere || isShoot) {
		object->SetModel(ModelManager::GetModel("playerSphere"));
	}
	else {
		object->SetModel(ModelManager::GetModel("player"));
	}


	//�G�t�F�N�g����
	if (attackFlag == true) {
		if (waveTime > 2) {
			waveIndex++;
			if (waveIndex >= 4) {
				waveIndex = 0;
			}
			waveright[waveIndex]->SetPosition(object->GetPosition());
			waveright[waveIndex]->VecSetPosition({ +5,0,0 });
			waveleft[waveIndex]->SetPosition(object->GetPosition());
			waveleft[waveIndex]->VecSetPosition({ -5,0,0 });
			waveTime = 0;
		}
		waveTime++;
	}
	else {
		for (int i = 0; i < 4; i++) {
			waveright[i]->SetPosition({ 0,-100,0 });
			waveleft[i]->SetPosition({ 0,-100,0 });
		}
	}
}

void Player::Res(bool flag, XMFLOAT3 vec)
{
	//object->VecSetPosition(backVec);
	move = move + backVec;

	//���~����
	if (resFlag2 == true) {
		if (gravity <= 2.0f) {
			gravity += 0.03f;
		}
		object->SetPosY({ object->GetPosition().y - gravity });
		if (object->GetPosition().y <= groundHeight) {
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
		crawAttackFlag = false;
		posY = object->GetPosition().y;
		resFlag1 = true;
		timer = 0;
		rollingSpeed = 0;
		backVec = Use::LoadXMVECTOR(-XMVector3Normalize(XMLoadFloat3(&move)));
	}
}

void Player::StopIn()
{
	rollingSpeed = 0;
	attackFlag = false;
	object->SetPosition({ 0,-6.0f,0 });
	resFlag1 = false;
	resFlag2 = false;
	gravity = 0.0f;
	backVec = { 0,0,0 };
}

void Player::StopOut()
{
	rollingSpeed = 0;
	rollingPower = 0;
	attackFlag = false;
	resFlag1 = false;
	resFlag2 = false;
	gravity = 0.0f;
	backVec = { 0,0,0 };
}

void Player::Draw()
{
	if (attackFlag) {
		for (int i = 0; i < waveNum; i++) {
			waveright[i]->Draw();
			waveleft[i]->Draw();
		}
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

bool Player::GetRes()
{
	if (resFlag1 == true || resFlag2 == true) {
		return true;
	}
	return false;
}

void Player::HitCrowAttack(XMFLOAT3 pos)
{
	if (pos.y == -100) {
		return;
	}
	if (crawAttackFlag == false) {
		crawTargetPos = pos;
		crawAttackFlag = true;

		XMVECTOR pos1, pos2, direction;
		pos1.m128_f32[0] = object->GetPosition().x;
		pos1.m128_f32[1] = object->GetPosition().y;
		pos1.m128_f32[2] = object->GetPosition().z;
		pos2.m128_f32[0] = crawTargetPos.x;
		pos2.m128_f32[1] = crawTargetPos.y;
		pos2.m128_f32[2] = crawTargetPos.z;
		direction = pos1 - pos2;
		direction = XMVector3Normalize(direction);
		crawDirection = Use::LoadXMVECTOR(direction);

		resFlag1 = false;
		resFlag2 = false;
		gravity = 0;
		backVec = { 0,0,0 };
	}
}

void Player::CrowAttack()
{
	if (crawAttackFlag == false) { return; }

	XMVECTOR pos1, pos2, direction;
	pos1.m128_f32[0] = crawTargetPos.x;
	pos1.m128_f32[1] = crawTargetPos.y;
	pos1.m128_f32[2] = crawTargetPos.z;
	pos2.m128_f32[0] = object->GetPosition().x;
	pos2.m128_f32[1] = object->GetPosition().y;
	pos2.m128_f32[2] = object->GetPosition().z;
	direction = pos1 - pos2;
	direction = XMVector3Normalize(direction);
	crawDirection = Use::LoadXMVECTOR(direction);

	object->SetPosition(object->GetPosition() + crawDirection * 2);

	//�Ƃ肠�����^�[�Q�b�g�ɂ����G�ɍU����������܂ŏ���
	attackFlag = true;
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

void Player::YazirusiUpdate()
{
	InputMouse* mouse = InputMouse::GetInstance();

	//�N���b�N�������n�߂̃E�B���h�E���W��ۑ�
	if (mouse->TorigerMouse(MouseDIK::M_LEFT)) {
		XMFLOAT2 mousepoint = mouse->GetWindowPos();
	}

	//�N���b�N�������Ă���Ԃ̍��W��ۑ�
	if (mouse->PushMouse(MouseDIK::M_LEFT)) {
		XMFLOAT2 mousepointnow = mouse->GetWindowPos();
	}

	//��_����p�x�����߂Ċp�x��ۑ�
	XMFLOAT2 m = { 0,0 };



	//���X�v���C�g�̉�]����ŋ��߂��p�x�ɂ���
	 

	//�G�t�F�N�g�̌������v�Z
	XMVECTOR ppos1 = XMLoadFloat2(&yazirusi->GetPosition()), ppos2 = XMLoadFloat2(&yazirusi->GetPosition());
	ppos2 += attackDirection * 6.0f;
	const float direction = 180.0f;
	XMFLOAT3 distance = Use::LoadXMVECTOR(ppos1 - ppos2);
	float angleToPlayer = (atan2(distance.x, distance.z)) * 180.0f / 3.14f + direction;
	yazirusi->SetRotation(angleToPlayer);

	if (InputMouse::GetInstance()->ReleaseMouse(MouseDIK::M_LEFT)) {
		yazirusiScale = { 1,1 };
	}

	XMFLOAT2 rotapoint = InputMouse::GetInstance()->GetScreanPos();
	float rotadistance = Collision::CheckDistance(rotapoint, clickTrigerPos);
	float yaziscale = 1 + rotadistance / 100;

	yazirusi->SetPosition(clickTrigerPos);
	yazirusi->SetScale({ yaziscale ,yaziscale });
}

