#include "GameScene.h"
#include "BatlleScene.h"
#include "SceneManager.h"
#include "StrongZakoEnemy.h"
#include "tuyoEnemy.h"
#include "safe_delete.h"
#include "ModelManager.h"
#include "Collision.h"

DirectX::XMFLOAT3 initTarget = { 0,-10,20 };
DirectX::XMFLOAT3 initEye = { 0,20,-25 };
using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	//�X�v���C�g���
	safe_delete(canvas);
	safe_delete(spriteBG);
	safe_delete(clearsprite);
	//safe_delete(postEffect);

	//3D�I�u�W�F�N�g���
	//safe_delete(objectFBX);
	//safe_delete(fbxmodel);
	safe_delete(kabe);
	safe_delete(kabe2);
	safe_delete(tenQ);
	safe_delete(ground);
	safe_delete(castle);
	safe_delete(suana);
	safe_delete(suana2);
	safe_delete(defenseTower);
	safe_delete(player);
	//safe_delete(copyPlayer);
	//safe_delete(copyCastle);
	//safe_delete(copyGround);
	//safe_delete(copyDefenseTower);
}

void GameScene::Initialize(DirectXCommon* dxCommon)
{
	assert(dxCommon);
	this->dxCommon = dxCommon;	

	//�J�����̏����ʒu�A�����_
	mainCamera = GameCamera::Create();
	mainCamera->SetTarget(initTarget);
	mainCamera->SetEye(initEye);
	ObjectObj::SetCamera(mainCamera);

	//�X�v���C�g�̐���
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	clearsprite = Sprite::Create(2, { 100.0f,100.0f });	

	//3D�I�u�W�F�N�g�̐���
	ground = ObjectObj::Create(ModelManager::GetModel("ground"));
	ground->SetScale({ 10.0f,1.0f,10.0f });
	ground->SetPosition({ 0.0f,-10.0f,0.0f });
	castle = ObjectObj::Create(ModelManager::GetModel("castle"));
	castle->SetScale({ 10.0f,10.0f,10.0f });
	suana = ObjectObj::Create(ModelManager::GetModel("suana"));
	suana->SetPosition({ 100.0f,0.0f,100.0f });
	suana->SetScale({ 10.0f,10.0f,10.0f });
	suana->SetRotation({ 0,90,0 });
	suana2 = ObjectObj::Create(ModelManager::GetModel("suana"));
	suana2->SetPosition({ -100.0f,0.0f,-100.0f });
	suana2->SetScale({ 10.0f,10.0f,10.0f });
	suana2->SetRotation({ 0,-90,0 });
	kabe = ObjectObj::Create(ModelManager::GetModel("kabe"));
	kabe->SetPosition({ 70.0f,-5.0f,50.0f });
	kabe->SetScale({ 5.0f,5.0f,5.0f });
	kabe2 = ObjectObj::Create(ModelManager::GetModel("kabe"));
	kabe2->SetPosition({ -60.0f,-5.0f,-50.0f });
	kabe2->SetScale({ 5.0f,5.0f,5.0f });
	kabe2->SetRotation({ 0,180,0 });
	tenQ = ObjectObj::Create(ModelManager::GetModel("tenQ"));
	tenQ->SetScale({ 5,5,5 });

	//�v���C���[�̐�������
	player = Player::Create(mainCamera);	
	player->object->SetPosition({ 0.0f,-6.0f,-50.0f });

	//�G�̐�������
	//std::unique_ptr<EnemyZako> enemy1 = std::make_unique<EnemyZako>();
	//enemy1->Initialize(EnemyZako::FIELD_OUT, camera, { -50, EnemyZako::groundOutPos,0 });
	//enemy1->SetPlayer(player);
	//enemy1->object->SetRotation({ 0,90,0 });
	//enemiesG.push_back(std::move(enemy1));
	enemiesG.clear();

	//�^���[�̐�������
	defenseTower = DefenseTower::Create();
	defenseTower->GetObjectOBJ()->SetPosition({ 20,0,20 });

	//�|�X�g�G�t�F�N�g�̐�������
	postEffect = new PostEffect();
	postEffect->Initialize();

	//�L�����o�X�̐�������
	canvas = new Canvas();
	canvas->Initialize();

	index = 0;
	timer[0] = 0;
	timer[1] = 160;
	timer[2] = 100;
	timer[3] = 100;
	timer[4] = 210;
	timer[5] = 200;

	dasuteki[0] = 2;
	dasuteki[1] = 1;
	dasuteki[2] = 1;
	dasuteki[3] = 1;
	dasuteki[4] = 2;
	dasuteki[5] = 1;

	Player::breakEnemy = 0;

	subCamera = Camera::Create();
	subCamera->SetEye({ 0, 100, -1 });
	subCamera->SetTarget({ 0, 0, 0 });
	copyPlayer = new CopyObject;
	copyPlayer->InitializeC(player->object);
	copyGround = new CopyObject;
	copyGround->InitializeC(ground);

	copyCastle = new CopyObject;
	copyCastle->InitializeC(castle);
	copyDefenseTower = new CopyObject;
	copyDefenseTower->InitializeC(defenseTower->GetObjectOBJ());

	//postEffect->SetSize({ 100,100 });

	postEffect->SetSize({ 1,1 });
	mainCamera->SetPlayer(player->object);

	//postEffect->SetColor({ 0.5,0.5,0.5});
}

void GameScene::Update(int& sceneNo, BatlleScene* batlleScene)
{
	if (Input::GetInstance()->TriggerKey(DIK_B))
	{
		blackFlag = true;
	}

	if (blackFlag == true) {
		blackTime -= 0.05;
		if (blackTime > 0) {
			postEffect->SetColor({ blackTime,blackTime,blackTime });
		}
		else {
			blackTime2 += 0.05f;
			postEffect->SetColor({ blackTime2,blackTime2,blackTime2 });
		}
		if (blackTime2 >= 1) {
			blackFlag = false;
			blackTime = 1.0f;
			blackTime2 = 0.0f;
		}
	}
	
	

	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		if (enemy->GetHp() <= 0) {
			Player::breakEnemy += 1;
		}
	}
	enemiesG.remove_if([](std::shared_ptr<EnemyZako>& enemy) {return enemy->GetDead(); });

	if (Input::GetInstance()->PushKey(DIK_UP)) {
		//spriteBG->SetSize({ 2, 2 });
		XMFLOAT2 postSize = postEffect->GetSize();
		postSize.x += 0.1;
		postSize.y += 0.1;
		postEffect->SetSize(postSize);
	}
	else if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		//spriteBG->SetSize({ 100, 100 });
		//postEffect->SetSize({ 100,100 });
		XMFLOAT2 postSize = postEffect->GetSize();
		postSize.x -= 0.1;
		postSize.y -= 0.1;
		postEffect->SetSize(postSize);
	}


	//�G��������
	timer[index]--;

	if (timer[index] <= 0) {
		if (dasuteki[index] == 1) {
			//�^���[�������
			std::shared_ptr<EnemyZako> newEnemy = std::make_shared<EnemyZako>();
			newEnemy->Initialize(EnemyZako::FIELD_OUT, mainCamera, { suana->GetPosition().x,EnemyZako::groundOutPos,suana->GetPosition().z }, true, XMFLOAT3{ 0, 0, -100 }, XMFLOAT3{ -100,0,0 });
			enemiesG.push_back(std::move(newEnemy));
		}
		if (dasuteki[index] == 2) {
			std::shared_ptr<EnemyZako> newEnemy = std::make_shared<EnemyZako>();
			newEnemy->Initialize(EnemyZako::FIELD_OUT, mainCamera, { suana2->GetPosition().x,EnemyZako::groundOutPos,suana2->GetPosition().z }, true, XMFLOAT3{ 0, 0, +100 }, XMFLOAT3{ +100,0,0 });
			enemiesG.push_back(std::move(newEnemy));
		}

		index++;
	}


	//�G���s�������邩�����Ȃ����̃g���O�X�C�b�`
	//EnemyZako::Action();

	////�����P����G�𐶐�
	//if (Input::GetInstance()->TriggerKey(DIK_1)) {
	//	std::shared_ptr<EnemyZako> newEnemy = std::make_shared<EnemyZako>();
	//	newEnemy->Initialize(EnemyZako::FIELD_OUT, camera, { suana->GetPosition().x,EnemyZako::groundOutPos,suana->GetPosition().z }, true, suana->GetPosition() + XMFLOAT3{ 0, 0, -50 });
	//	enemiesG.push_back(std::move(newEnemy));
	//}
	////�����Q����G�𐶐�
	//if (Input::GetInstance()->TriggerKey(DIK_2)) {
	//	std::shared_ptr<EnemyZako> newEnemy = std::make_shared<EnemyZako>();
	//	newEnemy->Initialize(EnemyZako::FIELD_OUT, camera, { suana->GetPosition().x,EnemyZako::groundOutPos,suana->GetPosition().z }, true, suana2->GetPosition() + XMFLOAT3{ 0, 0, 50 });
	//	enemiesG.push_back(std::move(newEnemy));
	//}
	//if (Input::GetInstance()->TriggerKey(DIK_3)) {
	//	std::shared_ptr<StrongZakoEnemy> newEnemy = std::make_shared<StrongZakoEnemy>();
	//	newEnemy->Initialize(EnemyZako::FIELD_OUT, camera, { suana->GetPosition().x,EnemyZako::groundOutPos,suana->GetPosition().z }, true, suana2->GetPosition() + XMFLOAT3{ 0, 0, 50 });
	//	enemiesG.push_back(std::move(newEnemy));
	//}

	//�G�ƃv���C���[�̓����蔻��
	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		if (CollisitonBoxToBox(enemy->object->GetPosition(), { 2.5,5,1 }, player->object->GetPosition(), { 5,5,5 })) {
			if (enemy->GetDead() == false) {
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
		}
		//�G�Ə�̓����蔻��
		if (CollisitonBoxToBox(enemy->object->GetPosition(), { 2.5,5,1 }, castle->GetPosition(), { 10,10,10 })) {
			//���������畉��
			//sceneNo = SceneManager::SCENE_END;
		}
	}
	canvas->SetEnemy(maxEnemy, player->breakEnemy);
	canvas->SetHp(player->GetMaxHp(), player->GetHp());

	//3D�I�u�W�F�N�g�X�V
	player->Update();
	ground->Update();
	defenseTower->Update(enemiesG);
	castle->Update();
	suana->Update();
	suana2->Update();
	kabe->Update();
	kabe2->Update();
	tenQ->Update();
	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		enemy->Update();
	}
	if (enemiesG.size() <= 0 && index >= 5) {
		sceneNo = SceneManager::SCENE_KATI;
	}

	//�J�����̃A�b�v�f�[�g
	mainCamera->Update();
	subCamera->Update();


	copyPlayer->Update(player->object, subCamera);
	copyGround->Update(ground, subCamera);
	copyCastle->Update(castle, subCamera);
	copyDefenseTower->Update(defenseTower->GetObjectOBJ(), subCamera);
	PostReserve();	//�~�j�}�b�v�̕`��O����
	subCamera->SetTarget(player->object->GetPosition());
	subCamera->SetEye({ player->object->GetPosition().x + 1,player->object->GetPosition().y + 100, player->object->GetPosition().z });

}

void GameScene::Draw()
{
	if (Input::GetInstance()->TriggerKey(DIK_M)) {
		if (mapFlag == true) { mapFlag = false; }
		else { mapFlag = true; }
	}
	if (mapFlag == true) {
		PostDraw();	//�~�j�}�b�v�̕`��
	}

	ObjectObj::PreDraw(dxCommon->GetCmdList());
	//objectFBX->Draw(dxCommon->GetCmdList());
	for (std::shared_ptr<EnemyZako>& enemy : enemiesG) {
		enemy->Draw();
	}
	ground->Draw();
	defenseTower->Draw();
	castle->Draw();
	suana->Draw();
	suana2->Draw();
	kabe->Draw();
	kabe2->Draw();
	tenQ->Draw();
	player->Draw();
	ObjectObj::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	//spriteBG->Draw();
	canvas->Draw();
	Sprite::PostDraw();
}

void GameScene::PostReserve()
{
	postEffect->PreDrawScene(dxCommon->GetCmdList());

	//�|�X�g�G�t�F�N�g���������I�u�W�F�N�g
	ObjectObj::PreDraw(dxCommon->GetCmdList());
	copyPlayer->Draw();
	copyGround->Draw();
	copyCastle->Draw();
	copyDefenseTower->Draw();
	ObjectObj::PostDraw();

	//�|�X�g�G�t�F�N�g���������X�v���C�g
	Sprite::PreDraw(dxCommon->GetCmdList());
	spriteBG->Draw();
	Sprite::PostDraw();

	postEffect->PosDrawScene(dxCommon->GetCmdList());
}

void GameScene::PostDraw()
{
	postEffect->Draw(dxCommon->GetCmdList());
}
