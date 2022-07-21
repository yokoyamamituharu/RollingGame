#pragma once
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>
#include <d3dx12.h>

class Camera
{
private://�G�C���A�X
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	static XMMATRIX matView;
	static XMMATRIX matProjection;
	static XMFLOAT3 eye;	//���_���W
	static XMFLOAT3 target;	//�����_���W
	static XMFLOAT3 up;		//������x�N�g��

	float angle = 0.0f;//�J�����̉�]�p

public:
	static bool StaticInitialize(int window_width, int window_height);

public:
	static void CameraMoveVector(XMFLOAT3 pos);

	//static void SetEye(XMFLOAT3 eye);




public:
	static XMMATRIX GetMatView() { return matView; };
	static XMMATRIX GetMatProjection() { return matProjection; };
	static XMFLOAT3 GetEye() { return eye; };
	static XMFLOAT3 GetTarget() { return target; };
	static XMFLOAT3 GetUp() { return up; };

public:
	static void CameraRoteVector(XMFLOAT3 move);

	static void SetCameraPostion(XMFLOAT3 Eye, XMFLOAT3 Target);


private:
	static void SetTarget(XMFLOAT3 target);
	static void SetEye(XMFLOAT3 eye);

	static void UpdateViewMatrix();
private:



};