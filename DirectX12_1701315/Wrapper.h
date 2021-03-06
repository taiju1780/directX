#pragma once
#include <d3d12.h>
#include "d3dx12.h"
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <vector>
#include <memory>
#include <Effekseer.h>
#include <EffekseerRendererDX12.h>

class Camera;
class PMDModel;
class PMXModel;
class Floor;
class Shadow;

//ペラポリゴン(マルチパス用)
struct VertexTex {
	DirectX::XMFLOAT3 pos;//頂点座標
	DirectX::XMFLOAT2 uv;
};

struct General {
	float time = 0;
	unsigned int depthfieldflag = false;
};

class Wrapper
{
private:

	ID3D12Device* _dev = nullptr;

	IDXGIFactory6* factory = nullptr;

	//RTVデスクリプタヒープ関係
	ID3D12DescriptorHeap* _rtvDescHeap = nullptr;
	std::vector<ID3D12Resource*> _backBuffers;

	///RTV用のデスクリプターヒープ初期化
	void InitDescriptorHeapRTV();

	//スワップチェイン
	///スワップチェイン初期化
	void InitSwapChain();
	IDXGISwapChain4* _swapchain = nullptr;

	//コマンド
	///コマンド初期化
	void InitCommand();
	ID3D12CommandAllocator* _cmdAllocator = nullptr;
	ID3D12GraphicsCommandList* _cmdList = nullptr;
	ID3D12CommandQueue* _cmdQue = nullptr;
	void ExecuteCmd();

	//フェンス
	///フェンス初期化
	void InitFence();
	UINT64 _fenceValue;//この値で待機するか執行するか判断する
	ID3D12Fence* _fence = nullptr; 
	///Queueが判断してこの関数で値を更新する
	void WaitExcute();

	////シェーダ関連
	void InitShader();
	
	////パイプライン
	void InitPipeline();

	//ビューポート、シザー
	D3D12_VIEWPORT _viewport;
	D3D12_RECT _scissorRect;

	//画像関係(歪み用テクスチャ)
	void InitTexture();
	ID3D12Resource* _texbuff;
	
	ID3D12DescriptorHeap* _texrtvHeap = nullptr;
	ID3D12DescriptorHeap* _texsrvHeap = nullptr;
	
	//画像関係(ノイズ用テクスチャ)
	void InitNoiseTexture();
	ID3D12Resource* _noisetexbuff;
	
	ID3D12DescriptorHeap* _noisetexrtvHeap = nullptr;
	ID3D12DescriptorHeap* _noisetexsrvHeap = nullptr;
	
	//画像関係(歪み用ノーマルマップ)
	void InitNormalTexture();
	ID3D12Resource* _normaltexbuff;
	
	ID3D12DescriptorHeap* _normaltexrtvHeap = nullptr;
	ID3D12DescriptorHeap* _normaltexsrvHeap = nullptr;

	//深度
	void InitDescriptorHeapDSV();
	ID3D12Resource* _dsvBuff;
	ID3D12DescriptorHeap* _dsvHeap = nullptr;
	ID3D12DescriptorHeap* _depthSrvHeap = nullptr;

	//カメラ
	std::shared_ptr<Camera> _camera;

	//pmdmodel
	std::shared_ptr<PMDModel> _model;

	//pmxmodel
	std::shared_ptr<PMXModel> _pmxModel;
	std::vector<std::shared_ptr<PMXModel>> _pmxModels;

	//Floar
	std::shared_ptr<Floor> _floor;

	//shadow
	std::shared_ptr<Shadow> _shadow;

	//ペラポリ
	D3D12_VERTEX_BUFFER_VIEW _1stvbView = {};
	ID3D12Resource* _peraBuff;

	//１パス目に使用するレンダリングバッファ
	std::vector<ID3D12Resource*> _1stPathBuffers;
	ID3D12DescriptorHeap* _rtv1stDescHeap = nullptr;		//RTV(レンダーターゲット)デスクリプタヒープ
	ID3D12DescriptorHeap* _srv1stDescHeap = nullptr;		//その他(テクスチャ、定数)デスクリプタヒープ

	ID3DBlob* peravertexShader = nullptr;
	ID3DBlob* perapixelShader = nullptr;

	ID3D12PipelineState* _perapipeline = nullptr;
	ID3D12RootSignature* _perarootsigunature = nullptr;

	void InitPath1stRTVSRV();
	void InitVerticesPera();
	void InitPath1stRootSignature();

	//ペラ2ポリ
	D3D12_VERTEX_BUFFER_VIEW _2ndvbView = {};
	ID3D12Resource* _pera2Buff;

	//2パス目に使用するレンダリングバッファ
	std::vector<ID3D12Resource*> _2ndPathBuffers;
	ID3D12DescriptorHeap* _rtv2ndDescHeap = nullptr;		//RTV(レンダーターゲット)デスクリプタヒープ
	ID3D12DescriptorHeap *_srv2ndDescHeap = nullptr;		//その他(テクスチャ、定数)デスクリプタヒープ

	ID3DBlob* pera2vertexShader = nullptr;
	ID3DBlob* pera2pixelShader = nullptr;

	ID3D12PipelineState* _pera2pipeline = nullptr;
	ID3D12RootSignature* _pera2rootsigunature = nullptr;

	void InitPath2ndRTVSRV();
	void InitVertices2Pera();
	void InitPath2ndRootSignature();

	//ペラ3ポリ
	D3D12_VERTEX_BUFFER_VIEW _3rdvbView = {};
	ID3D12Resource* _pera3Buff;

	//2パス目に使用するレンダリングバッファ
	std::vector<ID3D12Resource*> _3rdPathBuffers;
	ID3D12DescriptorHeap* _rtv3rdDescHeap = nullptr;		//RTV(レンダーターゲット)デスクリプタヒープ
	ID3D12DescriptorHeap *_srv3rdDescHeap = nullptr;		//その他(テクスチャ、定数)デスクリプタヒープ

	ID3DBlob* pera3vertexShader = nullptr;
	ID3DBlob* pera3pixelShader = nullptr;

	ID3D12PipelineState* _pera3pipeline = nullptr;
	ID3D12RootSignature* _pera3rootsigunature = nullptr;

	void InitPath3rdRTVSRV();
	void InitVertices3Pera();
	void InitPath3rdRootSignature();

	//影
	void DrawLightView();

	//ファイルパス
	std::string modelPath;
	std::string motionPath;

	//effekseer
	void InitEffekseer();
	Effekseer::Manager* efkManager;
	Effekseer::Effect* effect;
	EffekseerRenderer::Renderer* efkRenderer;
	EffekseerRenderer::SingleFrameMemoryPool* efkMemoryPool;
	EffekseerRenderer::CommandList* efkCmdList;
	Effekseer::Handle efkHandle;

	//imgui
	void InitIMGUI(HWND hwnd);
	ID3D12DescriptorHeap* imguiHeap;

	float clearColor[3];
	int InstanceNum;

	//定数バッファ(変数用)
	void InitConstantBuff();
	General general;
	ID3D12Resource *_Generalbuff;
	ID3D12DescriptorHeap* generalHeap;
	General* _vBufferptr = nullptr;

public:
	Wrapper(HINSTANCE h, HWND hwnd);
	~Wrapper();
	void Update();
	void PeraUpdate();
	void Pera2Update();
	void Pera3Update();
};

