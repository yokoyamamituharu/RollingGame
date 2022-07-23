cbuffer cbuff0 : register(b0)
{
	matrix viewproj;
	matrix world;
	float3 cameraPos;
};

struct VSInput
{
	float4 pos		: POSITION;
	float3 normal	: NORMAL;
	float2 uv		: TEXCOORD;
    uint4 boneIndices : BONEINDICES;	//�{�[���̔ԍ�
    float4 boneWeights : BONEWEIGHTS;	//�{�[���̃X�L���E�F�C�g
};

struct VSOutput
{
	float4 svpos  : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv	  : TEXCOORD;
    float4 worldPos : POSITION;
};

//�{�[���̍ő吔
static const int MAX_BONES = 32;

cbuffer skinning : register(b3)
{
    matrix matSkinning[MAX_BONES];
};

