#include"Camera.h"

using namespace DirectX;

//�l����
XMMATRIX Camera::matView{};
XMMATRIX Camera::matProjection{};
XMFLOAT3 Camera::eye = { 0, 0, -50 };
XMFLOAT3 Camera::target = { 0, 0, 0 };
XMFLOAT3 Camera::up = { 0, 1, 0 };


bool Camera::StaticInitialize(int window_width, int window_height)
{
	//�r���[�s��̐���
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	
	//�ˉe�ϊ��s��(�������e)
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		(float)window_width / window_height,
		0.1f, 1000.0f
	);

	return true;
}

void Camera::CameraMoveVector(XMFLOAT3 pos)
{
	XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	eye_moved.x += pos.x;
	eye_moved.y += pos.y;
	eye_moved.z += pos.z;

	target_moved.x += pos.x;
	target_moved.y += pos.y;
	target_moved.z += pos.z;

	eye = eye_moved;
	target = target_moved;
	UpdateViewMatrix();
}

void Camera::SetEye(XMFLOAT3 eye)
{
	Camera::eye = eye;

	UpdateViewMatrix();
}

void Camera::SetTarget(XMFLOAT3 target)
{
	Camera::target = target;

	UpdateViewMatrix();
}


void Camera::UpdateViewMatrix()
{
	//�r���[�s��̍X�V
	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void Camera::CameraRoteVector(XMFLOAT3 move)
{
	//XMFLOAT3 eye_moved = GetEye();
	XMFLOAT3 target_moved = GetTarget();

	XMMATRIX rotmat = XMMatrixIdentity();
	rotmat *= XMMatrixRotationY((move.x * 3.141592f * 2) / 1000);
	rotmat *= XMMatrixRotationX((move.y * XM_PI * 2) / 1000);

	XMVECTOR vector = XMLoadFloat3(&target_moved);

	XMVECTOR holdvector = XMVector3Transform(vector, rotmat);

	XMStoreFloat3(&target_moved, holdvector);


	/*XMFLOAT3 upTarget = { 0.0f, 1.0f, 0.0f };
	avector = XMLoadFloat3(&upTarget);

	aaa = XMVector3Transform(avector, rotmat);

	XMStoreFloat3(&up, aaa);*/

	SetTarget(target_moved);
}

void Camera::SetCameraPostion(XMFLOAT3 Eye, XMFLOAT3 Target)
{
	eye = Eye;
	target = Target;

	UpdateViewMatrix();
}