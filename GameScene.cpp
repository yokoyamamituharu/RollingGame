#include "GameScene.h"
DirectX::XMFLOAT3 initTarget = { 0,-10,20 };
DirectX::XMFLOAT3 initEye = { 0,20,-25 };

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	//3Dオブジェクト解放
	delete fbxobject;
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, InputMouse* mouse, Camera* camera)
{
	assert(dxCommon);
	this->dxCommon = dxCommon;
	this->input = input;
	this->mouse = mouse;
	this->camera = camera;

	//カメラの初期位置、注視点

	camera->SetTarget(initTarget);
	camera->SetEye(initEye);

	//スプライト共通テクスチャ読み込み
	Sprite::LoadTexture(0, L"Resources/texture.png");
	Sprite::LoadTexture(1, L"Resources/torisetu.png");

	//スプライトの生成
	spriteBG = Sprite::Create(0, { 0.0f,0.0f });


	//モデルの読み込み
	fbxmodel = FbxLoader::GetInstance()->LoadModelFromFile("cube");
	playermodel = new Model();
	playermodel->CreateFromOBJ("player");
	playerSpheremodel = new Model();
	playerSpheremodel->CreateFromOBJ("playerSphere");
	grundmodel = new Model();
	grundmodel->CreateFromOBJ("ground");
	//3Dオブジェクトの生成
	fbxobject = new FBXObject;
	fbxobject->Initialize();
	fbxobject->SetModel(fbxmodel);
	fbxobject->PlayAnimetion();
	fbxobject->SetPos({ 0,0,+80 });
	player = new Player();
	player->Initialize(input, mouse);
	player->object->SetScale({ 1.0f,1.0f,1.0f });
	player->object->SetPosition({ 0.0f,0.0f,0.0f });
	player->object->SetRotation({ 0.0f,90.0f,0.0f });
	ground = OBJobject::Create();
	ground->SetModel(grundmodel);
	ground->SetScale({ 10.0f,1.0f,10.0f });
	ground->SetPosition({ 0.0f,-10.0f,0.0f });
	ground->SetRotation({ 0.0f,0.0f,0.0f });

}

void GameScene::Update()
{
	//カメラの移動
	XMFLOAT3 playervec = { 0,0,0 };
	XMFLOAT3 cameravec = { 0,0,0 };
	float num = 0.8;
	if (input->PushKey(DIK_W)) {
		playervec.z += num;
	}
	if (input->PushKey(DIK_S)) {
		playervec.z -= num;
	}
	if (input->PushKey(DIK_A)) {
		playervec.x -= num;
	}
	if (input->PushKey(DIK_D)) {
		playervec.x += num;
	}

	if (input->PushKey(DIK_Q)) {
		playervec.y -= num;
	}
	if (input->PushKey(DIK_E)) {
		playervec.y += num;
	}

	if (mouse)


		player->MoveVector(playervec);

	//カメラの更新
	camera->SetEye({ player->object->GetPosition().x + initEye.x,player->object->GetPosition().y + initEye.y,
		player->object->GetPosition().z + initEye.z });
	camera->SetTarget({ player->object->GetPosition().x + initTarget.x,player->object->GetPosition().y + initTarget.y,
		player->object->GetPosition().z + initTarget.z });

	camera->Update();

	//3Dオブジェクト更新
	fbxobject->Update();
	player->Update();
	ground->Update();
}

void GameScene::Draw()
{
	Sprite::PreDraw(dxCommon->GetCmdList());
	//spriteBG->Draw();
	Sprite::PostDraw();

	OBJobject::PreDraw(dxCommon->GetCmdList());
	player->object->Draw();
	ground->Draw();
	//fbxobject->Draw(dxCommon->GetCmdList());

	OBJobject::PostDraw();

}
