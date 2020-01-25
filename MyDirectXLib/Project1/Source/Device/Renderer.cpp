#include "Renderer.h"
#include <d3d11.h>
#include <fbxsdk.h>
#include <DirectXMath.h>

#include "Def\Screen.h"

#include "Component\Graphics\SpriteRenderer.h"

#include "Device\Camera.h"
#include "Device\DirectXManager.h"
#include "Device\ShaderManager.h"

#include "Device\Base\SpriteVertex.h"
#include "Device\Base\ModelVertex.h"
#include "Device\Buffer\VertexBuffer.h"
#include "Device\Buffer\IndexBuffer.h"
#include "Device\Buffer\ConstantBuffer.h"

#include "Device\Buffer\WVPConstantBuffer.h"
#include "Device\TextureManager.h"

#include "Math\MathUtility.h"

float x = 0.0f;

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
	m_pSpriteInputLayout->Release();
	delete m_pSpriteVertices;
	delete m_pSpriteIndices;

	m_pRenderTexDefault->Release();

	m_pRTVDefault->Release();
}

void Renderer::init()
{
	initBuffers();
	initRenderTargets();
}

void Renderer::draw()
{
	auto pDevice = DirectXManager::getDevice();
	auto pDeviceContext = DirectXManager::getDeviceContext();

	float clearColor[4] = { 0, 0.5f, 0.75f, 1 };

	//�����_�[�^�[�Q�b�g���N���A
	pDeviceContext->ClearRenderTargetView(m_pRTVDefault, clearColor);

	//�ʏ�`��p�����_�[�^�[�Q�b�g���Z�b�g
	pDeviceContext->OMSetRenderTargets(1, &m_pRTVDefault, NULL);

	//drawSprites();
	drawMeshes();

	DirectXManager::presentSwapChain();
}

void Renderer::addSprite(SpriteRenderer * pSprite)
{
	int myDrawOrder = pSprite->getDrawOrder();

	auto itr = m_Sprites.begin();
	while (itr != m_Sprites.end())
	{
		if (myDrawOrder < (*itr)->getDrawOrder())
			break;

		++itr;
	}

	m_Sprites.insert(itr, pSprite);
}

void Renderer::removeSprite(SpriteRenderer * pSprite)
{
	auto itr = std::find(m_Sprites.begin(), m_Sprites.end(), pSprite);
	m_Sprites.erase(itr);
}

void Renderer::initBuffers()
{
#pragma region Sprite�p
	D3D11_INPUT_ELEMENT_DESC inputDesc[2];
	SpriteVertex::getInputDesc(inputDesc);
	UINT numElements = 2;

	auto pDevice = DirectXManager::getDevice();

	ShaderManager::GetVertexShader("SpriteDefault")->createInputLayout(
		pDevice,
		inputDesc,
		2,
		&m_pSpriteInputLayout
	);

	float width = 0.5f;
	float height = 0.5f;

	//�l�p�̒��_���쐬
	SpriteVertex vertices[]
	{
		{{-width,  height, 0.0f}, {0.0f, 0.0f}},
		{{ width, -height, 0.0f}, {1.0f, 1.0f}},
		{{-width, -height, 0.0f}, {0.0f, 1.0f}},
		{{ width,  height, 0.0f}, {1.0f, 0.0f}},
	};

	m_pSpriteVertices = new VertexBuffer();
	m_pSpriteVertices->init(pDevice, sizeof(SpriteVertex) * 4, vertices);

	UINT indices[]
	{
		0, 1, 2,
		0, 3, 1
	};

	m_pSpriteIndices = new IndexBuffer();
	m_pSpriteIndices->init(pDevice, sizeof(UINT) * 6, indices);
#pragma endregion
}

void Renderer::initRenderTargets()
{
	auto pSwapChain = DirectXManager::getSwapChain();
	auto pDevice = DirectXManager::getDevice();
	auto pDeviceContext = DirectXManager::getDeviceContext();

	//�r���[�|�[�g��ݒ�
	D3D11_VIEWPORT vp;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = (float)Screen::getWindowWidth();
	vp.Height = (float)Screen::getWindowHeight();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	pDeviceContext->RSSetViewports(1, &vp);

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	texDesc.Width = (float)Screen::getWindowWidth();
	texDesc.Height = (float)Screen::getWindowHeight();
	texDesc.CPUAccessFlags = 0;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;

	//�����_�[�e�N�X�`���쐬
	pDevice->CreateTexture2D(&texDesc, NULL, &m_pRenderTexDefault);	//�ʏ�`��

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ZeroMemory(&rtvDesc, sizeof(rtvDesc));
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	//�����_�[�^�[�Q�b�g�쐬
	pDevice->CreateRenderTargetView(m_pRenderTexDefault, &rtvDesc, &m_pRTVDefault);		//�ʏ�`��

	//SRV�쐬
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	ZeroMemory(&viewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	viewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	viewDesc.Texture2D.MipLevels = 1;

	pDevice->CreateShaderResourceView(m_pRenderTexDefault, &viewDesc, &m_pSRVDefault);

	//SwapChain����o�b�N�o�b�t�@���擾
	ID3D11Texture2D* pBack;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBack);
	//��ʕ`��pRTV�쐬
	pDevice->CreateRenderTargetView(pBack, NULL, &m_pRTVDefault);

	pBack->Release();
}

void Renderer::drawSprites()
{
	auto pDeviceContext = DirectXManager::getDeviceContext();

	pDeviceContext->IASetInputLayout(m_pSpriteInputLayout);

	auto vertices = m_pSpriteVertices->getBuffer();
	UINT stride = sizeof(SpriteVertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &vertices, &stride, &offset);
	pDeviceContext->IASetIndexBuffer(m_pSpriteIndices->getBuffer(), DXGI_FORMAT_R32_UINT, 0);

	for (auto sprite : m_Sprites)
	{
		sprite->draw();
	}
}

void Renderer::drawMeshes()
{
	auto pDevice = DirectXManager::getDevice();
	auto pDeviceContext = DirectXManager::getDeviceContext();

	//Fbx�n�̊Ǘ��I�u�W�F�N�g���쐬
	FbxManager* pFbxManager = FbxManager::Create();
	//���o�͐ݒ�
	FbxIOSettings* pIOSetting = FbxIOSettings::Create(pFbxManager, IOSROOT);
	pFbxManager->SetIOSettings(pIOSetting);
	//�C���|�[�^�[
	FbxImporter* pImporter = FbxImporter::Create(pFbxManager, "");

	std::string fileName = "Assets/Models/tekitou.fbx";
	//fbx���C���|�[�g
	if (!pImporter->Initialize(fileName.c_str(), -1, pFbxManager->GetIOSettings()))
	{
		//���s������false���Ԃ�̂ŃG���[����
	}

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
	ModelVertex* vertices = new ModelVertex[pMesh->GetControlPointsCount()];
	//���b�V���̒��_��S�擾
	for (int i = 0; i < pMesh->GetControlPointsCount(); ++i)
	{
		vertices[i].m_Pos.x = (float)pMesh->GetControlPointAt(i)[0];	//���_��X�������擾
		vertices[i].m_Pos.y = (float)pMesh->GetControlPointAt(i)[1];	//���_��Y�������擾
		vertices[i].m_Pos.z = (float)pMesh->GetControlPointAt(i)[2];	//���_��Z�������擾
		vertices[i].m_Pos.w = 1.0f;
	}

	//���_�o�b�t�@�̍쐬
	D3D11_BUFFER_DESC verticesDesc;
	verticesDesc.ByteWidth = sizeof(ModelVertex) * pMesh->GetControlPointsCount();
	verticesDesc.Usage = D3D11_USAGE_DEFAULT;
	verticesDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	verticesDesc.CPUAccessFlags = 0;
	verticesDesc.MiscFlags = 0;
	verticesDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA verticesData;
	verticesData.pSysMem = vertices;

	ID3D11Buffer* pVertexBuffer;
	auto result = pDevice->CreateBuffer(&verticesDesc, &verticesData, &pVertexBuffer);

	//�C���f�b�N�X�o�b�t�@�̍쐬
	D3D11_BUFFER_DESC indicesDesc;
	indicesDesc.ByteWidth = sizeof(int) * pMesh->GetPolygonVertexCount();
	indicesDesc.Usage = D3D11_USAGE_DEFAULT;
	indicesDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indicesDesc.CPUAccessFlags = 0;
	indicesDesc.MiscFlags = 0;
	indicesDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA indicesData;
	indicesData.pSysMem = pMesh->GetPolygonVertices();

	int polyCount = pMesh->GetPolygonCount();

	std::vector<int> indexVector;
	for (int i = 0; i < pMesh->GetPolygonCount(); ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			indexVector.emplace_back(pMesh->GetPolygonVertex(i, j));
		}
	}

	ID3D11Buffer* pIndexBuffer;
	pDevice->CreateBuffer(&indicesDesc, &indicesData, &pIndexBuffer);

	VertexShader meshVS("Assets/Shaders/MeshVS.cso");
	PixelShader meshPS("Assets/Shaders/MeshPS.cso");

	//�C���v�b�g���C�A�E�g�̍쐬
	D3D11_INPUT_ELEMENT_DESC layout[] = 
	{
		{ "POSITION", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	ID3D11InputLayout* pInputLayout;
	meshVS.createInputLayout(pDevice, layout, 1, &pInputLayout);

	//�萔�o�b�t�@�̍쐬
	D3D11_BUFFER_DESC constantDesc;
	constantDesc.ByteWidth = sizeof(WVPConstantBuffer);
	constantDesc.Usage = D3D11_USAGE_DEFAULT;
	constantDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantDesc.CPUAccessFlags = 0;
	constantDesc.MiscFlags = 0;
	constantDesc.StructureByteStride = 0;

	WVPConstantBuffer constantBuffer;
	//�ϊ��s��̍쐬
	DirectX::XMMATRIX translate = DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX scale = DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f);
	DirectX::XMMATRIX rotate = DirectX::XMMatrixRotationRollPitchYaw(MathUtility::toRadian(x), MathUtility::toRadian(x += 10), 0.0f);
	DirectX::XMMATRIX world = scale * rotate * translate;
	DirectX::XMMATRIX wvp = DirectX::XMMatrixTranspose(world * Camera::getViewProjMatrix3D());

	DirectX::XMStoreFloat4x4(&constantBuffer.wvpMatrix, wvp);

	D3D11_SUBRESOURCE_DATA constantData;
	constantData.pSysMem = &constantBuffer;

	ID3D11Buffer* pWVPBuffer;
	pDevice->CreateBuffer(&constantDesc, &constantData, &pWVPBuffer);

	//���X�^���C�U�̍쐬
	D3D11_RASTERIZER_DESC rasterDesc;
	ZeroMemory(&rasterDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = TRUE;

	ID3D11RasterizerState* pRasterizer;
	pDevice->CreateRasterizerState(&rasterDesc, &pRasterizer);

	UINT stride = sizeof(ModelVertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &pVertexBuffer, &stride, &offset);
	pDeviceContext->IASetIndexBuffer(pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	pDeviceContext->IASetInputLayout(pInputLayout);
	pDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pDeviceContext->VSSetShader(meshVS.getShader(), NULL, 0);
	pDeviceContext->PSSetShader(meshPS.getShader(), NULL, 0);
	pDeviceContext->VSSetConstantBuffers(0, 1, &pWVPBuffer);
	pDeviceContext->RSSetState(pRasterizer);

	pDeviceContext->DrawIndexed(pMesh->GetPolygonVertexCount(), 0, 0);

	pFbxManager->Destroy();

	delete vertices;
}
