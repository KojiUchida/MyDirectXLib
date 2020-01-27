#include "Mesh.h"
#include <fbxsdk.h>
#include "Device\DirectXManager.h"
#include "Device\Base\MeshVertex.h"
#include "Device\Buffer\VertexBuffer.h"
#include "Device\Buffer\IndexBuffer.h"

#ifdef _DEBUG
#include <cassert>
#endif // _DEBUG

Mesh::Mesh()
	: m_pVertexBuffer(nullptr),
	m_pIndexBuffer(nullptr)
{
}

Mesh::~Mesh()
{
	delete m_pVertexBuffer;
	delete m_pIndexBuffer;
}

void Mesh::init(const char * filePath)
{
	//Fbx�n�̊Ǘ��I�u�W�F�N�g���쐬
	FbxManager* pFbxManager = FbxManager::Create();
	//���o�͐ݒ�
	FbxIOSettings* pIOSetting = FbxIOSettings::Create(pFbxManager, IOSROOT);
	pFbxManager->SetIOSettings(pIOSetting);
	//�C���|�[�^�[
	FbxImporter* pImporter = FbxImporter::Create(pFbxManager, "");

	std::string fileName = filePath;

	//fbx���C���|�[�g
	bool isLoaded = pImporter->Initialize(fileName.c_str(), -1, pFbxManager->GetIOSettings());

#ifdef _DEBUG
	//���s������false���Ԃ�̂ŃG���[����
	assert(isLoaded);
#endif

	//�V�[���̍쐬�ƃC���|�[�g
	FbxScene* pFbxScene = FbxScene::Create(pFbxManager, "myScene");
	pImporter->Import(pFbxScene);

	//�C���|�[�g���I������̂Ŕj��
	pImporter->Destroy();

	//���b�V���f�[�^
	FbxMesh* pMesh = nullptr;
	//���b�V���f�[�^�̎擾
	for (int i = 0; i < pFbxScene->GetRootNode()->GetChildCount(); ++i)
	{
		//�V�[�����̃m�[�h������擾
		FbxNode* pChildNode = pFbxScene->GetRootNode()->GetChild(i);

		//�m�[�h�̎�ނ����b�V���p�Ȃ烁�b�V�����擾
		if (pChildNode->GetNodeAttribute()->GetAttributeType() == FbxNodeAttribute::eMesh)
		{
			pMesh = pChildNode->GetMesh();
			break;
		}
	}

	//�R���g���[���|�C���g(���_)�̐��Ŕz�񐶐�
	MeshVertex* vertices = new MeshVertex[pMesh->GetPolygonVertexCount()];

	//���b�V���̒��_��S�擾
	for (int i = 0; i < pMesh->GetControlPointsCount(); ++i)
	{
		vertices[i].m_Pos.x = (float)pMesh->GetControlPointAt(i)[0];	//���_��X�������擾
		vertices[i].m_Pos.y = (float)pMesh->GetControlPointAt(i)[1];	//���_��Y�������擾
		vertices[i].m_Pos.z = (float)pMesh->GetControlPointAt(i)[2];	//���_��Z�������擾
		vertices[i].m_Pos.w = (float)pMesh->GetControlPointAt(i)[3];	//���_��W�������擾
	}

	//�@���x�N�g�����擾
	int count = 0;
	for (int i = 0; i < pMesh->GetPolygonCount(); ++i)
	{
		for (int j = 0; j < pMesh->GetPolygonSize(i); ++j)
		{
			FbxVector4 normal;
			pMesh->GetPolygonVertexNormal(i, j, normal);
			vertices[count].m_Normal.x = (float)normal[0];
			vertices[count].m_Normal.y = (float)normal[1];
			vertices[count].m_Normal.z = (float)normal[2];
			vertices[count].m_Normal.w = (float)normal[3];
			count++;
		}
	}

	auto pDevice = DirectXManager::getDevice();

	//���_�o�b�t�@�̍쐬
	if (m_pVertexBuffer != nullptr)
		delete m_pVertexBuffer;
	m_pVertexBuffer = new VertexBuffer();
	m_pVertexBuffer->init(pDevice, sizeof(MeshVertex) * pMesh->GetControlPointsCount(), vertices);

	//�C���f�b�N�X�o�b�t�@�̍쐬
	if (m_pIndexBuffer != nullptr)
		delete m_pIndexBuffer;
	m_pIndexBuffer = new IndexBuffer();
	m_pIndexBuffer->init(pDevice, sizeof(int) * pMesh->GetPolygonVertexCount(), pMesh->GetPolygonVertices());

	//���_�����擾
	m_VertexCount = pMesh->GetPolygonVertexCount();

	pFbxManager->Destroy();
	delete[] vertices;
}

const VertexBuffer & Mesh::getVertexBuffer()
{
	return *m_pVertexBuffer;
}

const IndexBuffer & Mesh::getIndexBuffer()
{
	return *m_pIndexBuffer;
}

int Mesh::getVertexCount()
{
	return m_VertexCount;
}
