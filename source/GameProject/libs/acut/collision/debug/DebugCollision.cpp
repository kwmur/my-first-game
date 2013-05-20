#if defined(DEBUG) | defined(_DEBUG)



//#include "../../acut.h"
#include "DebugCollision.h"
#include "../../exception/Exception.h"
#include "../../exception/ExceptionHandler.h"
#include "../../macro/V_THROW.h"
#include "DXUT.h"


namespace acut {


	// D3DXCreateSphere() の参考URI: http://tgws.fromc.jp/prog/dx9game/D3DXCreateSphere/
	//const UINT DebugCollision::s_SLICES = 24;
	//const UINT DebugCollision::s_STACKS = 24;
	const UINT DebugCollision::s_SLICES = 16;
	const UINT DebugCollision::s_STACKS = 8;


	/**
	 * @brief constructor
	 *
	 */
	DebugCollision::DebugCollision() :
		m_pMesh(0)
	{
		// メッシュのマテリアル白色
		// ディフューズ
		m_materials.Diffuse.a = 0.0f; // 透明度
		m_materials.Diffuse.r = 1.0f;
		m_materials.Diffuse.g = 1.0f;
		m_materials.Diffuse.b = 1.0f;

		// 環境光
		m_materials.Ambient = m_materials.Diffuse;

		// スペキュラ
		m_materials.Specular.r = 0.0f;
		m_materials.Specular.g = 0.0f;
		m_materials.Specular.b = 0.0f;

		// エミッシヴ
		m_materials.Emissive.r = 0.0f;
		m_materials.Emissive.g = 0.0f;
		m_materials.Emissive.b = 0.0f;

		// 光沢の度合い
		m_materials.Power = 0.0f;
	}


	/**
	 * @brief destructor
	 *
	 */
	DebugCollision::~DebugCollision()
	{
		this->finalize();
	}


	/**
	 * @brief meshの解放
	 *
	 */
	void DebugCollision::finalize()
	{
		this->release();
	}


	/**
	 * @brief meshの描画
	 *
	 */
	void DebugCollision::render()
	{
		if (0 == m_pMesh) {
			return;
		}

		LPDIRECT3DDEVICE9 pDevice = DXUTGetD3D9Device();
		if (0 == pDevice) {
			V_THROW(E_POINTER);
		}

		try {
			// ワイヤフレームに設定
			V_THROW(pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME));

			// 描画
			V_THROW(pDevice->SetMaterial(&m_materials));
			V_THROW(m_pMesh->DrawSubset(0));

			// ワイヤフレームから設定をデフォルトのD3DFILL_SOLIDにする
			V_THROW(pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID));
		} catch (acut::Exception& ex) {
			acut::ExceptionHandler::handleException(ex);
			V_THROW(pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID));
			throw ex;
		}
	}


	/**
	 * @brief メッシュを球体に変更する
	 *
	 * @param radius 球体の半径
	 *
	 */
	void DebugCollision::changeMeshSphere(const FLOAT radius)
	{
		this->release();

		V_THROW(D3DXCreateSphere(
			DXUTGetD3D9Device(),
			radius,
			s_SLICES, // 主軸を回転軸としたスライスの数
			s_STACKS, // 主軸に沿ったスタックの数
			&m_pMesh,
			NULL
		));
	}


	/**
	 * @brief meshの解放
	 *
	 */
	void DebugCollision::release()
	{
		SAFE_RELEASE(m_pMesh);
	}


};



#endif // #if defined(DEBUG) | defined(_DEBUG)
