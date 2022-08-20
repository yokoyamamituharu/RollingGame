#include "BatlleScene.h"

BatlleScene::BatlleScene()
{
}

BatlleScene::~BatlleScene()
{
}

void BatlleScene::Initialize(DirectXCommon* dxCommon, Input* input, InputMouse* mouse, Camera* camera,GameScene* gameScene)
{
	assert(dxCommon);
	this->dxCommon = dxCommon;
	this->input = input;
	this->mouse = mouse;
	this->camera = camera;

	this->player = gameScene->GetPlayer();

	groundmodel = new Model();
	groundmodel->CreateFromOBJ("battlegrund");
	ground = OBJobject::Create();
	ground->SetModel(groundmodel);
	ground->SetScale({ 10.0f,1.0f,10.0f });
	ground->SetPosition({ 0.0f,-10.0f,0.0f });
	ground->SetRotation({ 0.0f,0.0f,0.0f });


	enemymodel = new Model();
	enemymodel->CreateFromOBJ("enemy");
	for (int i = 0; i < 6; i++) {
		enemys[i] = new Enemy();
		enemys[i]->Initialize(input, mouse, camera);
		enemys[i]->object->SetScale({ 4.0f,4.0f,4.0f });
		enemys[i]->object->SetRotation({ 0.0f,90.0f,0.0f });
		alive[i] = true;
	}

	enemys[0]->object->SetPosition({ 0.0f,-3.0f,0.0f });
	enemys[1]->object->SetPosition({ 100.0f,-3.0f,50.0f });
	enemys[2]->object->SetPosition({ -50.0f,-3.0f,100.0f });
	enemys[3]->object->SetPosition({ 80.0f,-3.0f,200.0f });
	enemys[4]->object->SetPosition({ -200.0f,-3.0f,100.0f });
	enemys[5]->object->SetPosition({ 0.0f,-3.0f,50.0f });
}

void BatlleScene::Update()
{
	camera->Update();	
	ground->Update();
	player->Update();
	for (int i = 0; i < 6; i++)
	{
		enemys[i]->Update();
	}
}

void BatlleScene::Draw()
{
	Sprite::PreDraw(dxCommon->GetCmdList());
	//spriteBG->Draw();
	Sprite::PostDraw();

	OBJobject::PreDraw(dxCommon->GetCmdList());
	player->object->Draw();
	ground->Draw();
	//fbxobject->Draw(dxCommon->GetCmdList());

	for (int i = 0; i < 6; i++) {
		if (alive[i] == true) {
			enemys[i]->object->Draw();
		}
	}
	OBJobject::PostDraw();
}
