#include "Camera.h"
#include <DirectXTex.h>
#include "WinApp.h"

using namespace DirectX;

Camera::Camera()
{
}

Camera::~Camera()
{
}

Camera* Camera::Create()
{
	//カメラクラスのインスタンスを生成
	Camera* ins = new Camera();
	//初期化処理
	ins->Initialize();
	//生成したインスタンスを返す
	return ins;
}

void Camera::Initialize()
{
	this->window_width = WinApp::GetWindowSize().x;
	this->window_height = WinApp::GetWindowSize().y;

	eye = { 0,0,-50 };
	target = { 0,0,0 };
	up = { 0,1,0 };

	// ビュー行列の生成
	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up));

	// 平行投影による射影行列の生成
	//constMap->mat = XMMatrixOrthographicOffCenterLH(
	//	0, window_width,
	//	window_height, 0,
	//	0, 1);
	// 透視投影による射影行列の生成
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f, 5000.0f
	);

	UpdateViewMatrix();

	// ビュープロジェクションの合成
	matViewProjection = matView * matProjection;

	matRot = XMMatrixIdentity();
}

void Camera::Update()
{
	UpdateViewMatrix();
	// ビュープロジェクションの合成
	matViewProjection = matView * matProjection;
}

void Camera::UpdateViewMatrix()
{
	// ビュー行列の更新
	matView = XMMatrixIdentity();
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	if (parent != nullptr) {
		matView *= *parent;
	}
}
