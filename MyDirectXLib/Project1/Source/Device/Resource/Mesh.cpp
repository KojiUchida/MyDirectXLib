#include "Mesh.h"
#include <fbxsdk.h>
#include "Device\DirectXManager.h"
#include "Device\Base\MeshVertex.h"
#include "Device\Buffer\VertexBuffer.h"
#include "Device\Buffer\IndexBuffer.h"
#include "Device\Resource\TextureManager.h"
#include "Utility\StringUtility.h"
#include <vector>

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

void Mesh::init(const std::string filePath, const std::wstring textureName)
{
	m_TextureName = StringUtility::toString(textureName);
	if (textureName != L"Default")
	{
		TextureManager::loadTexture(textureName.c_str(), m_TextureName);
	}

	//Fbx�n�̊Ǘ��I�u�W�F�N�g���쐬
	FbxManager* pFbxManager = FbxManager::Create();
	//���o�͐ݒ�
	FbxIOSettings* pIOSetting = FbxIOSettings::Create(pFbxManager, IOSROOT);
	pFbxManager->SetIOSettings(pIOSetting);
	//�C���|�[�^�[
	FbxImporter* pImporter = FbxImporter::Create(pFbxManager, "");

	//fbx���C���|�[�g
	bool isLoaded = pImporter->Initialize(filePath.c_str(), -1, pFbxManager->GetIOSettings());

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

	initVertices(pMesh);

	pFbxManager->Destroy();
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

void Mesh::initVertices(FbxMesh* pMesh)
{
	FbxStringList uvSetList;
	pMesh->GetUVSetNames(uvSetList);
	auto uvElem = pMesh->GetElementUV(uvSetList.GetStringAt(0));

	//UV���W�̐��Ŕz�񐶐�
	MeshVertex* vertices = new MeshVertex[pMesh->GetPolygonVertexCount()];
	int* indices = new int[pMesh->GetPolygonVertexCount()];

	//���_�o�b�t�@�p�̏����擾
	int vertexCount = pMesh->GetPolygonVertexCount() - 1;
	for (int i = 0; i < pMesh->GetPolygonCount(); ++i)
	{
		for (int j = 0; j < pMesh->GetPolygonSize(i); ++j)
		{
			int vertexIndex = pMesh->GetPolygonVertex(i, j);

			indices[vertexCount] = vertexCount;
			vertices[vertexCount].m_Pos.x = -(float)pMesh->GetControlPointAt(vertexIndex)[0];	//���_��X�������擾
			vertices[vertexCount].m_Pos.y = (float)pMesh->GetControlPointAt(vertexIndex)[1];	//���_��Y�������擾
			vertices[vertexCount].m_Pos.z = (float)pMesh->GetControlPointAt(vertexIndex)[2];	//���_��Z�������擾

			FbxVector4 normal;
			pMesh->GetPolygonVertexNormal(i, j, normal);
			vertices[vertexCount].m_Normal.x = -(float)normal[0];	//�@����X�������擾
			vertices[vertexCount].m_Normal.y = (float)normal[1];	//�@����Y�������擾
			vertices[vertexCount].m_Normal.z = (float)normal[2];	//�@����Z�������擾

			FbxVector2 uv;
			bool isUnMapped = true;
			pMesh->GetPolygonVertexUV(i, j, uvSetList.GetStringAt(0), uv, isUnMapped);
			vertices[vertexCount].m_UV.x = (float)uv[0];	//UV��X�������擾
			vertices[vertexCount].m_UV.y = 1 - (float)uv[1];	//UV��Y�������擾

			vertexCount--;
		}
	}

	//���_���Ƃ̏����擾

	auto pDevice = DirectXManager::getDevice();

	//���_�o�b�t�@�̍쐬
	if (m_pVertexBuffer != nullptr)
		delete m_pVertexBuffer;
	m_pVertexBuffer = new VertexBuffer();
	m_pVertexBuffer->init(pDevice, sizeof(MeshVertex) * uvElem->GetIndexArray().GetCount(), vertices);

	//�s�v�ɂȂ����̂Œ��_�f�[�^�����
	delete vertices;

	//�C���f�b�N�X�o�b�t�@�̍쐬
	if (m_pIndexBuffer != nullptr)
		delete m_pIndexBuffer;
	m_pIndexBuffer = new IndexBuffer();
	m_pIndexBuffer->init(pDevice, sizeof(int) * uvElem->GetIndexArray().GetCount(), indices);

	//�s�v�ɂȂ����̂ŃC���f�b�N�X�f�[�^�����
	delete indices;

	//���_�����擾
	m_VertexCount = uvElem->GetIndexArray().GetCount();
}
