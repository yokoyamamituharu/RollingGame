#include "TScene.h"
#include "SceneManager.h"
#include "StrongZakoEnemy.h"
#include "tuyoEnemy.h"
#include "safe_delete.h"
#include "ModelManager.h"
#include "../Collider/Collision.h"
#include "Useful.h"
#include "SphereCollider.h"
#include "YowaiEnemy.h"
#include "Route.h"

using namespace DirectX;

TScene::TScene()
{
}

TScene::~TScene()
{
	//�X�v���C�g���
	safe_delete(canvas);
	safe_delete(spriteBG);
	safe_delete(clearsprite);
	safe_delete(pose);
	safe_delete(tikaiSprite);
	safe_delete(playerSprte);
	safe_delete(towerSprte);
	safe_delete(miniMapPost);
	//3D�I�u�W�F�N�g���
	//safe_delete(particleM);
	safe_delete(scene);
	safe_delete(defenseTower);
	safe_delete(player);
	safe_delete(copyGround);
	safe_delete(copyCastle);
	safe_delete(copyDefenseTower);
	safe_delete(copyPlayer);
	enemiesG.clear();
	dasu.clear();
	safe_delete(gameCamera);
	safe_delete(subCamera);
}

void TScene::Initialize(DirectXCommon* dxCommon)
{
	assert(dxCommon);
	this->dxCommon = dxCommon;

	//�J�����̏����ʒu�A�����_
	gameCamera = GameCamera::Create();
	gameCamera->SetTarget({ 0,-10,20 });
	gameCamera->SetEye({ 0,20,-25 });
	ObjectObj::SetCamera(gameCamera);

	//�X�v���C�g�̐���
	spriteBG = Sprite::Create(SpriteManager::torisetu, { 0.0f,0.0f });
	pose = Sprite::Create(SpriteManager::pose, { 0,0 });
	//�L�����o�X�̐�������
	canvas = new Canvas();
	canvas->Initialize();
	tikaiSprite = Sprite::Create(SpriteManager::tikai, { 300,-50 });


	collisionManager = CollisionManager::GetInstance();

	//3D�I�u�W�F�N�g�̐���
	kabe1 = TouchableObject::Create(ModelManager::GetModel("kabe"));
	kabe1->SetPosition({ 0,-30,1015 });

	//�^���[�̐�������
	defenseTower = DefenseTower::Create();
	defenseTower->GetObjectObj()->SetPosition({ 20,0,35 });
	defenseTower->GetObjectObj()->SetScale({ 15,15,15 });

	//�v���C���[�̐�������
	player = Player::Create(gameCamera, 1);
	player->SetHp(5);
	player->object->SetPosition({ -822,Player::groundHeight,-884 });
	Player::breakEnemy = 0;
	//�Q�[���J�����Ƀv���C���[���Z�b�g
	gameCamera->SetPlayer(player->object);

	//�~�j�}�b�v�p�J�����̐���
	subCamera = Camera::Create();
	subCamera->SetEye({ 0, 100, -1 });
	subCamera->SetTarget({ 0, 0, 0 });
	//�~�j�}�b�v�p�I�u�W�F�N�g�̏�����	
	//copyGround = CopyObject::Create(ground);
	//copyCastle = CopyObject::Create(scene->GetObjectObj("castle"));
	copyDefenseTower = CopyObject::Create(defenseTower->GetObjectObj());
	copyPlayer = CopyObject::Create(player->object);
	//�~�j�}�b�v�p�|�X�g�G�t�F�N�g��������
	miniMapPost = new PostEffect();
	miniMapPost->Initialize();
	miniMapPost->SetSize({ 1,1 });

	scene = new SceneLoader;
	scene->Initialize("level", &towers);
	playerSprte = Sprite::Create(SpriteManager::sprite_0001, { 0,0 });
	towerSprte = Sprite::Create(SpriteManager::sprite_0002, { 0,0 });

	Route::GetIns()->Set();

	//particleM = new Particle;
	//particleM->Initialize();

}

void TScene::Update(int& sceneNo)
{
	ObjectObj::SetCamera(gameCamera);
	if (SceneManager::hitEnemyToPlayer || SceneManager::WinBattle) {
		return;
	}
	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
		poseFlag = !poseFlag;
	}
	if (poseFlag == true) return;
	for (std::shared_ptr<BaseEnemy>& enemy : enemiesG) {
		if (enemy->GetHp() <= 0) {
			Player::breakEnemy += 1;
		}
	}

	//���S�t���O�������Ă���G�������A���̍ۂɎ��S�����o�̃p�[�e�B�N���𔭐�������
	for (std::shared_ptr<BaseEnemy>& enemy : enemiesG) {
		if (enemy->GetDead()) {
			for (int i = 0; i < 5; i++) {
				std::unique_ptr<ParticleTubu> particle = std::make_unique<ParticleTubu>();
				particle->obj = std::make_unique<ObjectObj>();
				particle->obj->Initialize(ModelManager::GetModel("particle"));
				particle->obj->SetScale({ 5,5,5 });
				particle->end_frame = rand() % 5 + 30;
				particle->position = enemy->object->GetPosition();
				//tubu->scale = { 10,10,10 };
				//const float rnd_vel = 0.1f;
				int rndVel = 3.0f;
				particle->velocity.x = rand() % (rndVel * 2) - rndVel;
				particle->velocity.y = rand() % (rndVel * 2) - rndVel;
				particle->velocity.z = rand() % (rndVel * 2) - rndVel;
				//particleM->Add(std::move(particle));
			}
		}
	}
	enemiesG.remove_if([](std::shared_ptr<BaseEnemy>& enemy) {return enemy->GetDead(); });

	//�y��
	for (int i = 0; i < 2; i++) {
		std::unique_ptr<ParticleTubu> particle = std::make_unique<ParticleTubu>();
		particle->obj = std::make_unique<ObjectObj>();
		particle->obj->Initialize(ModelManager::GetModel("cloudBurst"));
		int scale = rand() % 20 + 1;
		particle->startScale = rand() % 4 + 1;
		particle->obj->SetScale({ float(scale),float(scale),float(scale) });
		particle->obj->SetRotation({ 0,0,0 });
		//�v���C���[�̑����ɐ���
		particle->position = { player->object->GetPosition().x + rand() % 3 - 1, player->object->GetPosition().y - 4 ,player->object->GetPosition().z + rand() % 3 - 1 };
		particle->end_frame = rand() % 5 + 10;
		//particleM->Add(std::move(particle));
	}

	//�G��������
	//SpownEnemy();

	//�G�̓����蔻��
	for (std::shared_ptr<BaseEnemy>& enemy : enemiesG) {
		//���S���Ă���G�l�~�[�������ꍇ�X�L�b�v
		if (enemy->GetDead() == true) {
			continue;
		}
		//�G�ƃv���C���[
		if (Collision::CheckBox2Box(enemy->object->GetPosition(), { 2.5,5,1 }, player->object->GetPosition(), { 5,5,5 })) {
			//�o�g���V�[���ɍs������
			player->outPos = player->object->GetPosition();
			player->StopOut();
			//�v���C���[�����_�ɔ���������
			//player->object->SetPosition({ 0,-6,0 });
			SceneManager::hitEnemyToPlayer = true;
			SceneManager::BattleInit = true;
			//sceneNo = SceneManager::SCENE_BATTLE;
			break;
		}
		//�G�Ə邪�߂����ǂ���
		if (enemy->tikai == false) {
			if (Collision::CheckDistance(scene->GetObjectObj("castle")->GetPosition(), enemy->object->GetPosition()) < 200) {
				enemy->tikai = true;
				isTikai = true;
				tikaiStack.push_back(true);
			}
		}
		//�G�Ə�
		if (Collision::CheckBox2Box(enemy->object->GetPosition(), { 2.5,5,1 }, scene->GetObjectObj("castle")->GetPosition(), { 20,20,20 })) {
			//���������畉��
			//sceneNo = SceneManager::SCENE_END;
		}
	}

	//�G�Ə邪�߂��ꍇ�Ɍx�������o��
	if (tikaiStack.size() > 0) {
		isTikai = true;
		tikaiTime++;
		if (tikaiTime < 120) {
			tikaiSprite->SetPosition({ tikaiSprite->GetPosition().x,tikaiSprite->GetPosition().y - 1 });
		}
		else if (tikaiTime < 200) {
			int kyomu = 0;
		}
		else if (tikaiTime < 320) {
			tikaiSprite->SetPosition({ tikaiSprite->GetPosition().x,tikaiSprite->GetPosition().y + 1 });
		}
		else {
			tikaiTime = 0;
			isTikai = false;
			tikaiStack.pop_front();
		}
	}

	//3D�I�u�W�F�N�g�X�V
	player->UpdateOut(gameCamera);
	defenseTower->Update(enemiesG);
	scene->Update();
	//touchGround->Update();
	for (std::shared_ptr<BaseEnemy>& enemy : enemiesG) {
		enemy->UpdateOut();
		if (enemy->GetDead()) {
			for (int i = 0; i < 5; i++) {
				std::unique_ptr<ParticleTubu> particle = std::make_unique<ParticleTubu>();
				particle->obj = std::make_unique<ObjectObj>();
				particle->obj->Initialize(ModelManager::GetModel("particle"));
				particle->obj->SetScale({ 5,5,5 });
				particle->end_frame = rand() % 5 + 30;
				particle->position = enemy->object->GetPosition();
				//tubu->scale = { 10,10,10 };
				//const float rnd_vel = 0.1f;
				int rndVel = 3.0f;
				particle->velocity.x = rand() % (rndVel * 2) - rndVel;
				particle->velocity.y = rand() % (rndVel * 2) - rndVel;
				particle->velocity.z = rand() % (rndVel * 2) - rndVel;
				//particleM->Add(std::move(particle));
			}
		}
	}
	if (enemiesG.size() <= 0 && index >= 7) {
		sceneNo = SceneManager::SCENE_KATI;
	}

	for (std::shared_ptr<DefenseTower>& tower : towers) {
		tower->SetPlayer(player);
		tower->Update(enemiesG);
	}



	gameCamera->Update();
	gameCamera->UpdateView();
	player->object->Update();
	player->shadowObj->Update();
	//particleM->Update();

	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		kabe1->VecSetPosition({ 10,0,0 });
	}
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		kabe1->VecSetPosition({ -10,0,0 });
	}

	kabe1->Update();

	//�J�����̃A�b�v�f�[�g	
	subCamera->Update();

	//�L�����o�X�Ƀv���C���[�̏����Z�b�g
	canvas->SetEnemy(maxEnemy, player->breakEnemy);
	canvas->SetHp(player->GetMaxHp(), player->GetHp());
}

void TScene::Draw()
{
	if (Input::GetInstance()->TriggerKey(DIK_M)) {
		mapFlag = !mapFlag;
	}
	if (mapFlag == true) {
		PostDraw();	//�~�j�}�b�v�̕`��
	}

	ObjectObj::PreDraw(dxCommon->GetCmdList());
	for (std::shared_ptr<BaseEnemy>& enemy : enemiesG) {
		enemy->Draw();
	}
	for (std::shared_ptr<DefenseTower>& tower : towers) {
		tower->Draw();
	}
	scene->Draw();
	//defenseTower->Draw();
	player->Draw();
	kabe1->Draw();
	//particleM->Draw();
	ObjectObj::PostDraw();

	Sprite::PreDraw(dxCommon->GetCmdList());
	//spriteBG->Draw();
	canvas->Draw();
	if (poseFlag == true) {
		pose->Draw();
	}
	if (isTikai) {
		tikaiSprite->Draw();
	}
	if (InputMouse::GetInstance()->PushMouse(MouseDIK::M_LEFT)) {
		player->arrowSymbolSprite->Draw();
	}

	Sprite::PostDraw();
}

void TScene::PostReserve()
{
	miniMapPost->PreDrawScene(dxCommon->GetCmdList());

	//�|�X�g�G�t�F�N�g���������I�u�W�F�N�g
	ObjectObj::PreDraw(dxCommon->GetCmdList());
	copyPlayer->Draw();
	//copyGround->Draw();
	//copyCastle->Draw();
	//copyDefenseTower->Draw();
	ObjectObj::PostDraw();

	//�|�X�g�G�t�F�N�g���������X�v���C�g
	Sprite::PreDraw(dxCommon->GetCmdList());
	spriteBG->Draw();
	playerSprte->SetPosition({ player->object->GetPosition().x - gameCamera->GetEye().x ,player->object->GetPosition().z - gameCamera->GetEye().z });
	playerSprte->Draw();
	//towerSprte->SetPosition({ defenseTower->object->GetPosition().x - gameCamera->GetEye().x ,defenseTower->object->GetPosition().z - gameCamera->GetEye().z });
	towerSprte->Draw();
	Sprite::PostDraw();

	miniMapPost->PosDrawScene(dxCommon->GetCmdList());
}

void TScene::PostDraw()
{
	miniMapPost->Draw(dxCommon->GetCmdList());
}

void TScene::SpownEnemy()
{
	if (index < maxEnemy) {
		dasu[index].timer--;
		while (dasu[index].timer <= 0) {
			if (dasu[index].hole == HOLE1) {
				//�^���[�������
				std::shared_ptr<EnemyZako> newEnemy = EnemyZako::Create(true, Route::GetRoute(3));
				enemiesG.push_back(std::move(newEnemy));
			}
			if (dasu[index].hole == HOLE2) {
				std::shared_ptr<YowaiEnemy> newEnemy = YowaiEnemy::Create(true, Route::GetRoute(4));
				enemiesG.push_back(std::move(newEnemy));
			}
			index++;
			if (index >= maxEnemy) {
				break;
			}
		}
	}
}

void TScene::CreaEffect()
{
}

void TScene::GameEndEffect()
{
}
