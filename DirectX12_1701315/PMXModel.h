#pragma once
#include<vector>
#include<map>
#include<array>
#include<DirectXMath.h>
#include <dxgi1_6.h>
#include <d3d12.h>

//ヘッダ情報
struct PMXHeader {
	char magic[4];
	float version;
	char bitesize;
	char data[8];
};

//コメント類
struct ModelInfo {
	int ModelNamesize;
	int ModelNameEsize;
	int Commentsize;
	int CommentEsize;
};

#pragma pack(1)

//頂点情報
struct PMXVertex {
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT2 uv;
	DirectX::XMFLOAT4 addUv[4];
	char waitclass;
	int bone[4];
	float wait[4];
	DirectX::XMFLOAT3 sdefvec[3];
	float edge;
};

//マテリアル
struct Material {
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT3 specular;
	float specPower;
	DirectX::XMFLOAT3 ambient;
	unsigned char bitFlag;
	DirectX::XMFLOAT4 edgeColer;
	float edgesize;
	unsigned int textureIndex;
	unsigned int sphIndex;
	unsigned char sphmode;
	unsigned char toonflag;
	unsigned int toonidx;
	int face_vert_cnt;
};

//IK
struct IKData {
	int targetboneidx;
	int loop;
	float loopangle;
	int link;
	int linkboneidx;
	char anglelimit;
	DirectX::XMFLOAT3 bottomangle;
	DirectX::XMFLOAT3 topangle;
};

//ボーン
struct BoneInfo {
	DirectX::XMFLOAT3 pos;
	unsigned int parentbone;
	unsigned int translevel;
	unsigned short bitflag;
	DirectX::XMFLOAT3 offset;
	unsigned int toboneidx;
	unsigned int toparentidx;
	float grantrate;
	DirectX::XMFLOAT3 axisvec;
	DirectX::XMFLOAT3 axisXvec;
	DirectX::XMFLOAT3 axisZvec;
	unsigned int key;
	IKData ik;
};

//頂点モーフ
struct VertexMorph {
	int vertidx;
	DirectX::XMFLOAT3 offset;
};

//UVモーフ
struct UVMorph {
	int vertidx;
	DirectX::XMFLOAT4 offset;
};

//ボーンモーフ
struct BoneMorph {
	int vertidx;
	DirectX::XMFLOAT3 moveOffset;
	DirectX::XMFLOAT4 rollOffset;
};

//マテリアルモーフ
struct MaterialMorph {
	int vertidx;
	char offsetclass;
	DirectX::XMFLOAT4 diffuse;
	DirectX::XMFLOAT3 speculer;
	float specPow;
	DirectX::XMFLOAT3 Ambient;
	DirectX::XMFLOAT4 edgecolor;
	float edgesize;
	DirectX::XMFLOAT4 texPow;
	DirectX::XMFLOAT4 sphtexPow;
	DirectX::XMFLOAT4 toontexPow;
};

//グループモーフ
struct GroupMorph {
	int vertidx;
	float morphnum;
};

//モーフデータ
struct MorphData {
	char panel;
	char type;
	int offsetnum;
	VertexMorph vertMorph;
	UVMorph uvMorph;
	BoneMorph boneMorph;
	MaterialMorph matMorph;
	GroupMorph groupMorph;
};


#pragma pack()

struct PMXColor {
	DirectX::XMFLOAT4 diffuse_color;	//dr,dg,db : 減衰色
	DirectX::XMFLOAT3 specular_color;	//sr,sg,sb : 光沢色
	DirectX::XMFLOAT3 ambient;			//mr,mg,mb : 環境色(Ambient)
};

class PMXModel
{
private:
	//モデル読み込み
	void LoadModel(const char * filepath, ID3D12Device* _dev);

	//頂点情報
	std::vector<PMXVertex> _verticesData;

	//インデックス情報
	std::vector<unsigned int> _indexData;

	//テクスチャ用パス
	std::vector<std::string> _texpath;

	void CreateWhiteTexture(ID3D12Device* _dev);
	ID3D12Resource* _whiteTexbuff;
	void CreateBlackTexture(ID3D12Device* _dev);
	ID3D12Resource* _blackTexbuff;
	void CreateGraduation(ID3D12Device* _dev);

	void CreatModelTex(ID3D12Device * _dev); 

	std::vector<std::string> _texturePaths;
	std::vector<ID3D12Resource*> _TexBuff;
	std::vector<ID3D12Resource*> _TexBuffspa;
	std::vector<ID3D12Resource*> _TexBuffsph;

	//ボーン名
	//ボーンデータ
	std::vector<BoneInfo> _boneData;

	std::vector<std::wstring> _bonename;
	std::map<std::wstring, std::pair<int,BoneInfo>> _boneDataInfo;
	std::vector<std::wstring> _bonenameE;

	//グラボに渡すため
	std::vector<DirectX::XMMATRIX> _boneMatrices;

	void RotationBone(const std::string & boneName, const DirectX::XMFLOAT4 & q1, const DirectX::XMFLOAT4 & q2, float t);

	ID3D12Resource* _boneBuff;
	ID3D12DescriptorHeap* _boneHeap = nullptr;
	DirectX::XMMATRIX* mappedBoneMat;

	//モーフデータ
	std::vector<MorphData> _morphData;

	//マテリアルデータ
	std::vector<Material> _matData;

	std::vector<ID3D12Resource*> _matBuffs;
	PMXColor* mappedColor = nullptr;
	ID3D12DescriptorHeap* _matHeap = nullptr;

	void InitMaterial(ID3D12Device * _dev);

	//str->wstr
	std::wstring StringToWStirng(const std::string& str);

	std::string WStringToStirng(const std::wstring & str);

	//toon
	void InitToon(std::string path, ID3D12Device * _dev, size_t idx);

	std::string toonfilepath;

	ID3D12Resource* _gladTexBuff = nullptr;

	std::vector<ID3D12Resource*> _ToonBuff;

	std::array<char[100], 10> toonTexNames;

	std::string GetToonTexpathFromIndex(int idx, std::string folderpath);
	std::string GetOriginToonTexpathFromIndex(int idx, std::string folderpath);

public:
	PMXModel(const char * filepath, ID3D12Device* _dev);
	~PMXModel();
	void Update();
	std::vector<PMXVertex> GetverticesData();
	std::vector<unsigned int> GetindexData();
	std::vector<Material> GetmatData();
	ID3D12DescriptorHeap*& GetMatHeap();
	ID3D12DescriptorHeap*& GetBoneHeap();
	void InitBone(ID3D12Device* _dev);
};

