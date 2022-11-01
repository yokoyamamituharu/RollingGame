#include "GameScene.h"
#include "BatlleScene.h"
#include "SceneManager.h"

DirectX::XMFLOAT3 initTarget = { 0,-10,20 };
DirectX::XMFLOAT3 initEye = { 0,20,-25 };
using namespace DirectX;

//�����̂̓����蔻��
bool CubeCollision(XMFLOAT3 object1, XMFLOAT3 radius1, XMFLOAT3 object2, XMFLOAT3 radius2) {
	float disX1 = object1.x + radius1.x;
	float disX2 = object2.x - radius2.x;
	float disX3 = object1.x - radius1.x;
	float disX4 = object2.x + radius2.x;
	float disY1 = object1.y + radius1.y;
	float disY2 = object2.y - radius2.y;
	float disY3 = object1.y - radius1.y;
	float disY4 = object2.y + radius2.y;
	float disZ1 = object1.z + radius1.z;
	float disZ2 = object2.z - radius2.z;
	float disZ3 = object1.z - radius1.z;
	float disZ4 = object2.z + radius2.z;

	//x��y�͂��ꂼ�ꒆ�S���W�Ƃ��Čv�Z����
	return disX1 > disX2 && disX4 > disX3 && disY1 > disY2 && disY4 > disY3 && disZ1 > disZ2 && disZ4 > disZ3;
}

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	//3D�I�u�W�F�N�g���
	delete fbxobject;
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, InputMouse* mouse, Camera* camera)
{
	assert(dxCommon);
	this->dxCommon = dxCommon;
	this->input = input;
	this->mouse = mouse;
	this->camera = camera;

	//�J�����̏����ʒu�A�����_
	camera->SetTarget(initTarget);
	camera->SetEye(initEye);

	//�X�v���C�g���ʃe�N�X�`���ǂݍ���
	Sprite::LoadTexture(0, L"Resources/texture.png");
	Sprite::LoadTexture(1, L"Resources/torisetu.png");
	Sprite::LoadTexture(2, L"Resources/clear.png");

	//�X�v���C�g�̐���
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	clearsprite = Sprite::Create(2, { 100.0f,100.0f });


	//�e�̐ÓI������
	Bullet::StaticInitialize();

	//���f���̓ǂݍ���
	fbxmodel = FbxLoader::GetInstance()->LoadModelFromFile("cube");
	groundmodel = Model::Create("ground");
	enemymodel = Model::Create("enemy");
	castleModel = Model::Create("castle");

	//3D�I�u�W�F�N�g�̐���
	fbxobject = new FBXObject;
	fbxobject->Initialize();
	fbxobject->SetModel(fbxmodel);
	fbxobject->PlayAnimetion();
	fbxobject->SetPos({ 0,0,+80 });

	ground = OBJobject::Create();
	ground->SetModel(groundmodel);
	ground->SetScale({ 10.0f,1.0f,10.0f });
	ground->SetPosition({ 0.0f,-10.0f,0.0f });
	ground->SetRotation({ 0.0f,0.0f,0.0f });

	ground = OBJobject::Create();
	ground->SetModel(groundmodel);
	ground->SetScale({ 10.0f,1.0f,10.0f });
	ground->SetPosition({ 0.0f,-10.0f,0.0f });
	ground->SetRotation({ 0.0f,0.0f,0.0f });

	castle = OBJobject::Create();
	castle->SetModel(castleModel);
	castle->SetScale({ 10.0f,10.0f,10.0f });

	//�v���C���[�̐�������
	player = new Player();
	player->Initialize(input, mouse, camera);
	player->object->SetScale({ 1.0f,1.0f,1.0f });
	player->object->SetPosition({ 0.0f,-6.0f,-50.0f });
	player->object->SetRotation({ 0.0f,90.0f,0.0f });

	//�G�̐�������
	std::unique_ptr<EnemyZako> enemy1 = std::make_unique<EnemyZako>();	
	enemy1->Initialize(EnemyZako::FIELD_OUT, camera, { -50, EnemyZako::groundOutPos,0 });
	enemy1->SetPlayer(player);
	enemy1->object->SetRotation({ 0,90,0 });
	enemiesG.push_back(std::move(enemy1));

	//�^���[�̐�������
	defenseTower = DefenseTower::Create();
	defenseTower->GetOBJObject()->SetPosition({ 20,0,20 });
	bullet = Bullet::Create();

}

void GameScene::Update(int& sceneNo ,BatlleScene* batlleScene)
{
	//player->Update();
	//�J��������
	if (input->PushKey(DIK_RIGHT)) {
		camera->matRot *= XMMatrixRotationY(0.1f);
	}
	else if (input->PushKey(DIK_LEFT)) {
		camera->matRot *= XMMatrixRotationY(-0.1f);
	}

	if (Input::GetInstance()->PushKey(DIK_H)) {
		int num = 0;
	}
	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
		cameraToMouse *= -1;
	}

	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		sceneNo = 2;
	}


	//std::list<std::unique_ptr<EnemyZako>>enemies1 = std::move(enemy1->GetEnemies());

	//�G�ƃv���C���[�̓����蔻��
	for (std::unique_ptr<EnemyZako>& enemy : enemiesG) {
		if (CubeCollision(enemy->object->GetPosition(), { 2.5,5,1 }, player->object->GetPosition(), { 5,5,5 })) {
			//�o�g���V�[���ɍs������
			batlleScene->SetEnemies(enemy);
			enemiesG.remove(enemy);
			sceneNo = SceneManager::SCENE_BATTLE;
			break;
		}
	}



	//3D�I�u�W�F�N�g�X�V
	fbxobject->Update();
	player->Update();
	ground->Update();
	defenseTower->Update(player);
	bullet->Update();
	for (std::unique_ptr<EnemyZako>& enemy : enemiesG) {
		enemy->Update();
	}	
	castle->Update();

	if (cameraToMouse == 1) {
		camera->matRot *= XMMatrixRotationY(0.8f * mouse->MoveMouseVector('x') / 1000);
	}

	//XMFLOAT3 rote = player->object->GetRotation();
	XMFLOAT3 pos = player->object->GetPosition();
	XMVECTOR movement = { 0, 0, 1.0f, 0 };
	//XMMATRIX matRot = XMMatrixRotationY(XMConvertToRadians(rote.y));
	movement = XMVector3TransformNormal(movement, camera->matRot);
	movement *= XMVECTOR{ -1, -1, -1 };
	//matRot = XMMatrixRotationY((XMConvertToRadians(rote.y)));
	camera->SetEye({ player->object->GetPosition().x + movement.m128_f32[0] * 80, player->object->GetPosition().y + movement.m128_f32[1] * 80,
		 player->object->GetPosition().z + movement.m128_f32[2] * 80 });
	camera->eye.y = 20;
	camera->target = player->object->GetPosition();
	//�v���C���[���W�����v���������_������Ɍ����̂�h�~���邽�߂̏����G
	//camera->target.y = player->graundheight;



	//�J�����̍X�V
	//camera->SetEye({ player->object->GetPosition().x + initEye.x,player->object->GetPosition().y + initEye.y,
	//	player->object->GetPosition().z + initEye.z });
	//camera->SetTarget({ player->object->GetPosition().x + initTarget.x,player->object->GetPosition().y + initTarget.y,
	//	player->object->GetPosition().z + initTarget.z });
	camera->SetWorldMatrix(player->object->GetWorldMatrix());
	camera->Update();
}

void GameScene::Draw()
{
	OBJobject::PreDraw(dxCommon->GetCmdList());
	//fbxobject->Draw(dxCommon->GetCmdList());
	player->object->Draw();
	ground->Draw();
	defenseTower->Draw();
	bullet->Draw();

	for (std::unique_ptr<EnemyZako>& enemy : enemiesG) {
		enemy->Draw();
	}
	castle->Draw();

	OBJobject::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	//spriteBG->Draw();	
	Sprite::PostDraw();

}
