#pragma once

#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dx12.h>
#include <cstdlib>

#include "WinApp.h"

#include<d3d12.h>
#include<d3dx12.h>
#include<dxgi1_6.h>
#include<wrl.h>

#include<cassert>

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

using namespace Microsoft::WRL;

//DirectX�ėp
class DirectXCommon
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: // �����o�֐�
	//������
	void Initialize(WinApp *win);

	//�`��O����
	void PreDraw();
	//�`��㏈��
	void PostDraw();

	//�f�o�C�X�擾
	ID3D12Device *GetDev() { return device.Get(); }

	//�R�}���h���X�g�擾
	ID3D12GraphicsCommandList *GetCmdList() { return commandList.Get(); }

private: // �����o�ϐ�
	bool InitializeDXGIDevice();
	bool InitializeCommand();
	bool CreateSwapChain();
	bool CreateFinalRenderTargets();
	bool CreateDepthBuffer();
	bool CreateFence();

private: // �����o�֐�

private:
	
	ComPtr<IDXGIFactory6>dxgiFactory;
	ComPtr <ID3D12Device>device;
	ComPtr<ID3D12GraphicsCommandList>commandList = nullptr;
	ComPtr<ID3D12CommandAllocator>commandAllocator = nullptr;
	ComPtr<ID3D12CommandQueue>commandQueue = nullptr;
	ComPtr<IDXGISwapChain4>swapchain = nullptr;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	ComPtr<ID3D12Resource> depthBuffer;
	ComPtr<ID3D12DescriptorHeap>rtvHeaps = nullptr;
	ComPtr<ID3D12DescriptorHeap> dsvHeap = nullptr;
	ComPtr<ID3D12Fence> fence = nullptr;
	UINT64 fenceVal = 0;

	//WindowsAPI
	WinApp *winApp = nullptr;

};