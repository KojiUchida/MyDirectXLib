#pragma once

struct VertexBuffer;
struct IndexBuffer;

//���b�V���p�\����
struct Mesh
{
public:
	Mesh();
	~Mesh();

	//����������
	void init(const char* filePath);

	//���_�o�b�t�@�̎擾
	const VertexBuffer& getVertexBuffer();
	//�C���f�b�N�X�o�b�t�@�̎擾
	const IndexBuffer& getIndexBuffer();

	int getVertexCount();

private:
	//�R�s�[�֎~
	Mesh(const Mesh&) = delete;
	void operator = (const Mesh&) = delete;

private:
	VertexBuffer* m_pVertexBuffer;
	IndexBuffer* m_pIndexBuffer;
	int m_VertexCount;
};