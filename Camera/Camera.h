#pragma once
#include <DirectXMath.h>

class Camera
{
protected:
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using XMVECTOR = DirectX::XMVECTOR;

public:	//�����o�֐�
	Camera();
	~Camera();

	/// <summary>
	/// �J�����̐���
	/// </summary>
	/// <returns>���������J�����̃|�C���^�[</returns>
	static Camera* Create();

	/// <summary>
	/// �J�����N���X�̏���������
	/// </summary>
	void Initialize();

	/// <summary>
	/// ���t���[���X�V����
	/// </summary>
	virtual void Update();

	void UpdateMat();

	void UpdateView();

	//�㉺�̎��_�ړ�
	void VecUp(float scaler) {
		matRot *= DirectX::XMMatrixRotationY(0.8f * scaler);
	}

	//���E�̎��_�ړ�
	void VecSide(float scaler) {
		matRot *= DirectX::XMMatrixRotationX(0.8f * scaler);

	}

protected: //�����o�֐�
	/// <summary>
	/// �r���[�s��̍X�V
	/// </summary>
	void UpdateViewMatrix();

public:	//�Q�b�^�[
	/// <summary>
	/// �r���[�s����擾
	/// </summary>
	/// <returns>�r���[�s��</returns>
	XMMATRIX GetMatView() {
		return matView;
	}

	/// <summary>
	/// �ˉe�s����擾
	/// </summary>
	/// <returns>�ˉe�s��</returns>
	XMMATRIX GetMatProjection() {
		return matProjection;
	}

	/// <summary>
	/// �r���[�ˉe�s��̎擾
	/// </summary>
	/// <returns>�r���[�ˉe�s��</returns>
	XMMATRIX GetViewProjectionMatrix() {
		return matViewProjection;
	}

	/// <summary>
	/// ���_���W���擾
	/// </summary>
	/// <returns>���_���W</returns>
	XMFLOAT3 GetEye() {
		return eye;
	}

	/// <summary>
	/// �����_���W���擾
	/// </summary>
	/// <returns>�����_���W</returns>
	XMFLOAT3 GetTarget() {
		return target;
	}

	/// <summary>
	/// ������x�N�g�����擾
	/// </summary>
	/// <returns>������x�N�g��</returns>
	XMFLOAT3 GetUp() {
		return eye;
	}

public:	//�Z�b�^�[
	/// <summary>
	/// ���_���W���Z�b�g
	/// </summary>
	/// <param name="eye">�V�������_���W</param>
	void SetEye(XMFLOAT3 eye) { this->eye = eye; }
	/// <summary>
	/// �����_���W���Z�b�g
	/// </summary>
	/// <param name="target">�V���������_���W</param>
	void SetTarget(XMFLOAT3 target) { this->target = target; }

	/// <summary>
	/// ������x�N�g�����Z�b�g
	/// </summary>
	/// <param name="up">�V����������x�N�g��</param>
	void SetUp(XMFLOAT3 up) { this->target = up; }

	/// <summary>
	/// �e�ƂȂ�s����Z�b�g
	/// </summary>
	/// <param name="mat">�e�ƂȂ�s��</param>
	void SetParent(XMMATRIX& mat) { parent = &mat; }
	//void ClearParent() { parent = nullptr; }
	//void SetMatView(XMMATRIX mat) { matView = mat; }
	//void SetMatProjection(XMMATRIX mat) { matProjection = mat; }

public:	//�����o�ϐ�
	XMMATRIX matRot;
	// ���_���W
	XMFLOAT3 eye = { 0,0,-50 };
	// �����_���W
	XMFLOAT3 target = { 0,0,0 };
	// ������x�N�g��
	XMFLOAT3 up = { 0,1,0 };
protected: //�����o�ϐ�
	// �r���[�s��
	XMMATRIX matView;
	// �ˉe�s��
	XMMATRIX matProjection;
	//�r���[�ˉe�s��
	XMMATRIX matViewProjection;




	int window_width;
	int window_height;

	XMMATRIX* parent = nullptr;
};

