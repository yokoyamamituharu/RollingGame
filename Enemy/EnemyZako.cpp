#include "EnemyZako.h"
using namespace DirectX;

void EnemyZako::Initialize(Input* input, InputMouse* mouse, Camera* camera)
{
	assert(input);
	assert(mouse);
	this->input = input;
	this->mouse = mouse;
	this->camera = camera;
	//敵用モデルを読み込み
	enemyModel = new Model();
	enemyModel->CreateFromOBJ("enemy");
	//オブジェクトの作成
	object = OBJobject::Create();
	object->SetModel(enemyModel);
}

void EnemyZako::Update()
{
	Direction(player);
	//Move();
	if (flag1)
	{

		if (waitTime <= 0) {
			waitTime = rand() % 360 + 240;
			//waitTime = 60;
			num1 = waitTime;
		}
		if (waitTime > 0 && attackFlag == false)
		{
			//移動処理
			if (num1 >= waitTime) {
				num1 = waitTime - (rand() % 60 + 60);
				num2 = rand() % 2 + 1;
			}

			float a = 0.2f;
			if (num2 == 1) {
				move = XMFLOAT3(a, 0.0f, 0.0f);
			}
			else if (num2 == 2) {
				move = XMFLOAT3(-a, 0.0f, 0.0f);
			}
			else if (num2 == 3) {
				move = XMFLOAT3(0.0f, 0.0f, -a);
			}
			else if (num2 == 4) {
				move = XMFLOAT3(0.0f, 0.0f, a);
			}
			MoveVector(move);

			waitTime--;
			if (waitTime <= 0) {
				attackFlag = true;
				targetPos = player->object->GetPosition();
				timer1 = 120;
			}
		}

		if (attackFlag)
		{
			timer1--;
			GoTarget(targetPos);
			if (object->GetPosition().x == targetPos.x && object->GetPosition().z == targetPos.z || timer1 <= 0) {
				attackFlag = false;
			}
		}
	}

	//オブジェクトの更新
	object->Update();
}


void EnemyZako::GoTarget(XMFLOAT3 target)
{
	float speed = 10.0f;

	////攻撃用ローカル変数
	//XMFLOAT3 direction = object->GetPosition();
	////direction= XMVector3Normalize(direction);

	//enemypos = {direction.x* speed, direction.y* speed, direction.z* speed};
	//enemypos1 = { enemypos1.x * direction.x, enemypos1.y * direction.y, enemypos1.z * direction.z };
	//object->SetPosition(enemypos1);

	float posX = object->GetPosition().x;
	float posY = object->GetPosition().y;
	float posZ = object->GetPosition().z;
	float playerPosX = player->object->GetPosition().x;
	float playerPosZ = player->object->GetPosition().z;
	float distanceX = 0;
	float distanceZ = 0;
	//oldPlayerPos = player->object->GetPosition();

	distanceX = posX - target.x;
	distanceZ = posZ - target.z;

	posX -= distanceX / speed;
	posZ -= distanceZ / speed;

	object->SetPosition(XMFLOAT3(posX, posY, posZ));
}

void EnemyZako::Direction(Player* player)
{
	const float direction = 90.0f;
	XMFLOAT3 pos = object->GetPosition();
	XMFLOAT3 playerPos = player->object->GetPosition();

	XMFLOAT3 distance = { pos.x - playerPos.x,pos.y - playerPos.y,pos.z - playerPos.z };

	angle = (atan2(distance.x, distance.z)) * 180.0f / 3.14f + direction;

	object->SetRotation(XMFLOAT3(0.0f, angle, 0.0f));
}

void EnemyZako::Mawarikomi(Player* player)
{
	num -= 0.005;
	float x = object->GetPosition().x + 0.3f;
	float y = object->GetPosition().y;
	float z = object->GetPosition().z + num;

	object->SetPosition({ x, y, z });
}

