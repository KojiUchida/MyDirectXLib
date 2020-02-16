#pragma once
#include <vector>

class SpriteRenderer;
class MeshRenderer;
class VertexBuffer;
class IndexBuffer;
class ConstantBuffer;

struct ID3D11InputLayout;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct ID3D11ShaderResourceView;
struct ID3D11RasterizerState;
struct ID3D11SamplerState;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void init();
	void draw();

	//SpriteRenderer��o�^����B���������p�Ȃ̂Ŋ�{�g��Ȃ�
	void addSprite(SpriteRenderer* pSprite);
	//SpriteRenderer�̓o�^����������B���������p�Ȃ̂Ŋ�{�g��Ȃ�
	void removeSprite(SpriteRenderer* pSprite);

	//MeshRenderer��o�^����B���������p�Ȃ̂Ŋ�{�g��Ȃ�
	void addMesh(MeshRenderer* pMesh);
	//MeshRenderer�̓o�^����������B���������p�Ȃ̂Ŋ�{�g��Ȃ�
	void removeMesh(MeshRenderer* pMesh);

private:
	void initBuffers();
	void initRenderTargets();

	void drawSprites();
	void drawMeshes();

private:
	//���X�^���C�U
	ID3D11RasterizerState* m_pRasterizer;

	//�X�v���C�g�Ǘ��pvector
	std::vector<SpriteRenderer*> m_Sprites;

	//�X�v���C�g�p
	ID3D11InputLayout* m_pSpriteInputLayout;
	ID3D11SamplerState* m_pSpriteSampler;
	VertexBuffer* m_pSpriteVertices;
	IndexBuffer* m_pSpriteIndices;

	//3D���f���Ǘ��pvector
	std::vector<MeshRenderer*> m_Meshes;

	//3D���f���p
	ID3D11InputLayout* m_pMeshInputLayout;
	ID3D11SamplerState* m_pMeshSampler;

	//�����_�[�^�[�Q�b�g
	ID3D11Texture2D* m_pRenderTexDefault;
	ID3D11RenderTargetView* m_pRTVDefault;

	//�[�x�o�b�t�@
	ID3D11Texture2D* m_pDepthStencilTexture;
	ID3D11DepthStencilView* m_pDepthStencilView;
};