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
	//�J�����N���X�̃C���X�^���X�𐶐�
	Camera* ins = new Camera();
	//����������
	ins->Initialize();
	//���������C���X�^���X��Ԃ�
	return ins;
}

void Camera::Initialize()
{
	this->window_width = WinApp::GetWindowSize().x;
	this->window_height = WinApp::GetWindowSize().y;

	eye = { 0,0,-50 };
	target = { 0,0,0 };
	up = { 0,1,0 };

	// �r���[�s��̐���
	matView = XMMatrixLookAtLH(
		XMLoadFloat3(&eye),
		XMLoadFloat3(&target),
		XMLoadFloat3(&up));

	// ���s���e�ɂ��ˉe�s��̐���
	//constMap->mat = XMMatrixOrthographicOffCenterLH(
	//	0, window_width,
	//	window_height, 0,
	//	0, 1);
	// �������e�ɂ��ˉe�s��̐���
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f, 5000.0f
	);

	UpdateViewMatrix();

	// �r���[�v���W�F�N�V�����̍���
	matViewProjection = matView * matProjection;

	matRot = XMMatrixIdentity();
}

void Camera::Update()
{
	UpdateViewMatrix();
	// �r���[�v���W�F�N�V�����̍���
	matViewProjection = matView * matProjection;
}

void Camera::UpdateViewMatrix()
{
	// �r���[�s��̍X�V
	matView = XMMatrixIdentity();
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	if (parent != nullptr) {
		matView *= *parent;
	}
}
