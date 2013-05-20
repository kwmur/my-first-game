#include "Floor.h"
#include "../GameConfiguration.h"


/**
 * @brief constructor
 *
 */
Floor::Floor() :
	m_pMesh(NULL),
	m_numberOfMaterials(0)
{
	D3DXMatrixIdentity(&m_worldMatrix);
}


/**
 * @brief destructor
 *
 */
Floor::~Floor()
{
	// 特に処理は無し
}


/**
 * @brief Initialize floor
 *
 */
void Floor::initialize()
{
}


/**
 * @brief デバイス作成後に呼び出される
 *
 */
void Floor::onDeviceCreated()
{
	// mesh読み込み
	acut::MeshUtility::loadMeshFromX(
		&m_pMesh,
		&m_numberOfMaterials,
		&m_meshMaterials,
		&m_meshTexturePointers,
		GameConfiguration::s_FLOOR_MESH_FILE_PATH,
		GameConfiguration::s_FLOOR_MESH_FOLDER_PATH,
		true
	);
}


/**
 * @brief Direct3D device の解体後に呼ばれる
 *
 * onDeviceCreated() で取得したリソースをここで解放する
 *
 */
void Floor::onDeviceDestroyed()
{
	std::for_each(m_meshTexturePointers.begin(), m_meshTexturePointers.end(), acut::Release<LPDIRECT3DTEXTURE9>());
	m_meshTexturePointers.clear();
	SAFE_RELEASE(m_pMesh);
}


/**
 * @brief 床を描画する
 * 
 */
void Floor::render()
{
	LPDIRECT3DDEVICE9 pDevice = DXUTGetD3D9Device();
	V_THROW(pDevice->SetTransform(D3DTS_WORLD, &m_worldMatrix));
	for (DWORD i = 0; i < m_numberOfMaterials; ++i) {
		V_THROW(pDevice->SetMaterial(&m_meshMaterials.at(i)));
		V_THROW(pDevice->SetTexture(0, m_meshTexturePointers.at(i)));
		V_THROW(m_pMesh->DrawSubset(i));
	}
}

