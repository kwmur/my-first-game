#if defined(DEBUG) | defined(_DEBUG)



//#include "../../acut.h"
#include "DebugCollision.h"
#include "../../exception/Exception.h"
#include "../../exception/ExceptionHandler.h"
#include "../../macro/V_THROW.h"
#include "DXUT.h"


namespace acut {


	// D3DXCreateSphere() �̎Q�lURI: http://tgws.fromc.jp/prog/dx9game/D3DXCreateSphere/
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
		// ���b�V���̃}�e���A�����F
		// �f�B�t���[�Y
		m_materials.Diffuse.a = 0.0f; // �����x
		m_materials.Diffuse.r = 1.0f;
		m_materials.Diffuse.g = 1.0f;
		m_materials.Diffuse.b = 1.0f;

		// ����
		m_materials.Ambient = m_materials.Diffuse;

		// �X�y�L����
		m_materials.Specular.r = 0.0f;
		m_materials.Specular.g = 0.0f;
		m_materials.Specular.b = 0.0f;

		// �G�~�b�V��
		m_materials.Emissive.r = 0.0f;
		m_materials.Emissive.g = 0.0f;
		m_materials.Emissive.b = 0.0f;

		// ����̓x����
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
	 * @brief mesh�̉��
	 *
	 */
	void DebugCollision::finalize()
	{
		this->release();
	}


	/**
	 * @brief mesh�̕`��
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
			// ���C���t���[���ɐݒ�
			V_THROW(pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME));

			// �`��
			V_THROW(pDevice->SetMaterial(&m_materials));
			V_THROW(m_pMesh->DrawSubset(0));

			// ���C���t���[������ݒ���f�t�H���g��D3DFILL_SOLID�ɂ���
			V_THROW(pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID));
		} catch (acut::Exception& ex) {
			acut::ExceptionHandler::handleException(ex);
			V_THROW(pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID));
			throw ex;
		}
	}


	/**
	 * @brief ���b�V�������̂ɕύX����
	 *
	 * @param radius ���̂̔��a
	 *
	 */
	void DebugCollision::changeMeshSphere(const FLOAT radius)
	{
		this->release();

		V_THROW(D3DXCreateSphere(
			DXUTGetD3D9Device(),
			radius,
			s_SLICES, // �厲����]���Ƃ����X���C�X�̐�
			s_STACKS, // �厲�ɉ������X�^�b�N�̐�
			&m_pMesh,
			NULL
		));
	}


	/**
	 * @brief mesh�̉��
	 *
	 */
	void DebugCollision::release()
	{
		SAFE_RELEASE(m_pMesh);
	}


};



#endif // #if defined(DEBUG) | defined(_DEBUG)
