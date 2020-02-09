#pragma once
#include <string>

struct VertexBuffer;
struct IndexBuffer;

namespace fbxsdk
{
	class FbxMesh;
}

//���b�V���p�\����
struct Mesh
{
public:
	Mesh();
	~Mesh();

	//����������
	void init(const std::string filePath, const std::wstring textureName = L"Default");

	//���_�o�b�t�@�̎擾
	const VertexBuffer& getVertexBuffer();
	//�C���f�b�N�X�o�b�t�@�̎擾
	const IndexBuffer& getIndexBuffer();

	//�e�N�X�`�����̎擾
	std::string getTextureName() { return m_TextureName; };

	int getVertexCount();

private:
	void initVertices(fbxsdk::FbxMesh* pMesh);

private:
	//�R�s�[�֎~
	Mesh(const Mesh&) = delete;
	void operator = (const Mesh&) = delete;

private:
	VertexBuffer* m_pVertexBuffer;
	IndexBuffer* m_pIndexBuffer;
	int m_VertexCount;
	std::string m_TextureName;
};