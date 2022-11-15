#include "GameScene.h"
#include "BatlleScene.h"
#include "SceneManager.h"
#include "StrongZakoEnemy.h"
#include "/K020G1196/RollingGame_ryuunen/tuyoEnemy.h"

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
	Sprite::LoadTexture(3, L"Resources/hp.png");
	Sprite::LoadTexture(4, L"Resources/damageHp.png");
	//�w�i
	Sprite::LoadTexture(5, L"Resources/title.png");
	Sprite::LoadTexture(6, L"Resources/end.png");
	Sprite::LoadTexture(7, L"Resources/black.png");

	Sprite::LoadTexture(9, L"Resources/num/slash.png");
	Sprite::LoadTexture(10, L"Resources/num/0.png");
	Sprite::LoadTexture(11, L"Resources/num/1.png");
	Sprite::LoadTexture(12, L"Resources/num/2.png");
	Sprite::LoadTexture(13, L"Resources/num/3.png");
	Sprite::LoadTexture(14, L"Resources/num/4.png");
	Sprite::LoadTexture(15, L"Resources/num/5.png");
	Sprite::LoadTexture(16, L"Resources/num/6.png");
	Sprite::LoadTexture(17, L"Resources/num/7.png");
	Sprite::LoadTexture(18, L"Resources/num/8.png");
	Sprite::LoadTexture(19, L"Resources/num/9.png");


	//Sprite::LoadTexture(5, L"Resources/hp.png");

	//�X�v���C�g�̐���
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	clearsprite = Sprite::Create(2, { 100.0f,100.0f });


	//�e�̐ÓI������
	Bullet::StaticInitialize();

	EnemyZako::EnemyCreateModel();

	StrongZakoEnemy::CreateStrongEnemyModel();

	//���f���̓ǂݍ���
	fbxmodel = FbxLoader::GetInstance()->LoadModelFromFile("cube");
	groundmodel = Model::Create("ground");
	enemymodel = Model::Create("enemy");
	castleModel = Model::Create("castle");
	suanaModel = Model::Create("suana");
	kabeModel = Model::Create("kabe");
	tenqModel = Model::Create("tenQ");

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

	suana = OBJobject::Create();
	suana->SetModel(suanaModel);
	suana->SetPosition({ 100.0f,0.0f,100.0f });
	suana->SetScale({ 10.0f,10.0f,10.0f });
	suana->SetRotation({ 0,90,0 });

	suana2 = OBJobject::Create();
	suana2->SetModel(suanaModel);
	suana2->SetPosition({ -100.0f,0.0f,-100.0f });
	suana2->SetScale({ 10.0f,10.0f,10.0f });
	suana2->SetRotation({ 0,-90,0 });

	kabe = OBJobject::Create();
	kabe->SetModel(kabeModel);
	kabe->SetPosition({ 70.0f,-5.0f,30.0f });
	kabe->SetScale({ 5.0f,5.0f,5.0f });
	kabe->SetRotation({ 0,0,0 });

	kabe2 = OBJobject::Create();
	kabe2->SetModel(kabeModel);
	kabe2->SetPosition({ -60.0f,-5.0f,-50.0f });
	kabe2->SetScale({ 5.0f,5.0f,5.0f });
	kabe2->SetRotation({ 0,180,0 });

	tenQ = OBJobject::Create();
	tenQ->SetModel(tenqModel);
	tenQ->SetScale({ 5,5,5 });

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

	postEffect = new PostEffect();
	postEffect->Initialize();

	canvas = new Canvas();
	canvas->Initialize();

	index = 0;
	timer[0] = 0;
	timer[1] = 120;
	timer[2] = 60;
	timer[3] = 60;
	timer[4] = 210;
	timer[5] = 200;

	dasuteki[0] = 2;
	dasuteki[1] = 1;
	dasuteki[2] = 1;
	dasuteki[3] = 1;
	dasuteki[4] = 2;
	dasuteki[5] = 1;
}

void GameScene::Update(int& sceneNo, BatlleScene* batlleScene)
{
	//player->Update();
	//�J��������
	//if (input->PushKey(DIK_RIGHT)) {
	//	camera->matRot *= XMMatrixRotationY(0.1f);
	//}
	//else if (input->PushKey(DIK_LEFT)) {
	//	camera->matRot *= XMMatrixRotationY(-0.1f);
	//}

	//�G��������
	timer[index]--;

	if (timer[index] <= 0) {
		if (dasuteki[index] == 1) {
			//�^���[�������
			std::shared_ptr<EnemyZako> newEnemy = std::make_shared<EnemyZako>();
			newEnemy->Initialize(EnemyZako::FIELD_OUT, camera, { suana->GetPosition().x,EnemyZako::groundOutPos,suana->GetPosition().z }, true,  XMFLOAT3{ 0, 0, -100 }, XMFLOAT3{ -100,0,0 });
			enemiesG.push_back(std::move(newEnemy));
		}
		if (dasuteki[index] == 2) {
			std::shared_ptr<EnemyZako> newEnemy = std::make_shared<EnemyZako>();
			newEnemy->Initialize(EnemyZako::FIELD_OUT, camera, { suana2->GetPosition().x,EnemyZako::groundOutPos,suana2->GetPosition().z }, true, XMFLOAT3{ 0, 0, +100 }, XMFLOAT3{ +100,0,0 });
			enemiesG.push_back(std::move(newEnemy));
		}

		index++;
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

	//�G���s�������邩�����Ȃ����̃g���O�X�C�b�`
	EnemyZako::Action();

	//�����P����G�𐶐�
	if (Input::GetInstance()->TriggerKey(DIK_1)) {
		std::shared_ptr<EnemyZako> newEnemy = std::make_shared<EnemyZako>();
		newEnemy->Initialize(EnemyZako::FIELD_OUT, camera, { suana->GetPosition().x,EnemyZako::groundOutPos,suana->GetPosition().z }, true, suana->GetPosition() + XMFLOAT3{ 0, 0, -50 });
		enemiesG.push_back(std::move(newEnemy));
	}

	//�����Q����G�𐶐�
	if (Input::GetInstance()->TriggerKey(DIK_2)) {
		std::shared_ptr<EnemyZako> newEnemy = std::make_shared<EnemyZako>();
		newEnemy->Initialize(EnemyZako::FIELD_OUT, camera, { suana->GetPosition().x,EnemyZako::groundOutPos,suana->GetPosition().z }, true, suana2->GetPosition() + XMFLOAT3{ 0, 0, 50 });
		enemiesG.push_back(std::move(newEnemy));
	}

	if (Input::GetInstance()->TriggerKey(DIK_3)) {
		std::shared_ptr<StrongZakoEnemy> newEnemy = std::make_shared<StrongZakoEnemy>();
		newEnemy->Initialize(EnemyZako::FIELD_OUT, camera, { suana->GetPosition().x,EnemyZako::groundOutPos,suana->GetPosition().z }, true, suana2->GetPosition() + XMFLOAT3{ 0, 0, 50 });
		enemiesG.push_back(std::move(newEnemy));
	}


	//std::list<std::unique_ptr<EnemyZako>>enemies1 = std::move(enemy1->GetEnemies());

	//�G�ƃv���C���[�̓����蔻��
	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		if (CubeCollision(enemy->object->GetPosition(), { 2.5,5,1 }, player->object->GetPosition(), { 5,5,5 })) {
			//�o�g���V�[���ɍs������
			batlleScene->SetEnemies(enemy);
			enemiesG.remove(enemy);
			player->outPos = player->object->GetPosition();
			player->Stop();
			//�v���C���[�����_�ɔ���������
			player->object->SetPosition({ 0,-6,0 });
			sceneNo = SceneManager::SCENE_BATTLE;
			break;
		}

		//�G�Ə�̓����蔻��
		if (CubeCollision(enemy->object->GetPosition(), { 2.5,5,1 }, castle->GetPosition(), { 10,10,10 })) {
			//���������畉��
			sceneNo = SceneManager::SCENE_END;
		}
	}
	canvas->SetEnemy(maxEnemy, player->breakEnemy);
	canvas->SetHp(player->GetMaxHp(), player->GetHp());

	if (Input::GetInstance()->TriggerKey(DIK_PGUP)) {
		player->Cure(1);
	}
	if (Input::GetInstance()->TriggerKey(DIK_PGDN)) {
		player->Damage(1);
	}

	//3D�I�u�W�F�N�g�X�V
	fbxobject->Update();
	player->Update();
	ground->Update();

	bullet->Update();
	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		enemy->Update();
	}
	defenseTower->Update(enemiesG);

	castle->Update();

	suana->Update();
	suana2->Update();


	kabe->Update();
	kabe2->Update();
	tenQ->Update();


	//�J��������
	if (input->PushKey(DIK_RIGHT)) {
		camera->matRot *= XMMatrixRotationY(0.02f);
	}
	else if (input->PushKey(DIK_LEFT)) {
		camera->matRot *= XMMatrixRotationY(-0.02f);
	}
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
	ground->Draw();
	defenseTower->Draw();
	bullet->Draw();

	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		enemy->Draw();
	}
	castle->Draw();
	suana->Draw();
	suana2->Draw();
	kabe->Draw();
	kabe2->Draw();
	tenQ->Draw();
	player->object->Draw();
	OBJobject::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	//spriteBG->Draw();	
	canvas->Draw();
	Sprite::PostDraw();
}

void GameScene::PostReserve()
{
	postEffect->PreDrawScene(dxCommon->GetCmdList());

	//�|�X�g�G�t�F�N�g���������I�u�W�F�N�g
	OBJobject::PreDraw(dxCommon->GetCmdList());
	//player->object->Draw();
	OBJobject::PostDraw();

	//�|�X�g�G�t�F�N�g���������X�v���C�g
	Sprite::PreDraw(dxCommon->GetCmdList());
	//spriteBG->Draw();	
	Sprite::PostDraw();

	postEffect->PosDrawScene(dxCommon->GetCmdList());
}

void GameScene::PostDraw()
{
	postEffect->Draw(dxCommon->GetCmdList());
}
