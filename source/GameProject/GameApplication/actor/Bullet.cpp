#include "Bullet.h"
#include "../GameConfiguration.h"
#include "../GameApplication.h"



// static�����o�ϐ�
bool Bullet::s_shot = false;
double Bullet::s_lastShotTime = 0.0;
// mesh
LPD3DXMESH Bullet::s_pMesh = NULL;
DWORD Bullet::s_numberOfMaterials = 0;
std::vector<D3DMATERIAL9> Bullet::s_meshMaterials;
std::vector<LPDIRECT3DTEXTURE9> Bullet::s_meshTexturePointers;
// sound
CSound* Bullet::s_pSoundShot = 0;

// debug
#ifdef DEBUG
acut::DebugCollision Bullet::s_debugCollision;
#endif



/**
 * @brief constructor
 *
 */
Bullet::Bullet() :
	m_position(0.0f, 0.0f, 0.0f),
	m_previousPosition(0.0f, 0.0f, 0.0f),
	m_direction(0.0f, 0.0f, 0.0f),
	m_existenceTime(GameConfiguration::s_BULLET_DISAPPEARANCE_TIME)
{
}


/**
 * @brief destructor
 *
 */
Bullet::~Bullet()
{
}


/**
 * @brief ������
 *
 */
void Bullet::initialize()
{
}


/**
 * @brief �I������
 *
 */
void Bullet::finalize()
{
}


/**
 * @brief �f�o�C�X�쐬��ɌĂяo�����
 *
 */
void Bullet::onDeviceCreated()
{
	// mesh�ǂݍ���
	acut::MeshUtility::loadMeshFromX(
		&s_pMesh,
		&s_numberOfMaterials,
		&s_meshMaterials,
		&s_meshTexturePointers,
		GameConfiguration::s_BULLET_MESH_FILE_PATH,
		GameConfiguration::s_BULLET_MESH_FOLDER_PATH
	);

#ifdef DEBUG
	s_debugCollision.changeMeshSphere(GameConfiguration::s_BULLET_COLLISION_RADIUS);
#endif

	// sound
	s_pSoundShot = acut::SoundUtility::loadSound(
		&GameApplication::s_soundManager,
		GameConfiguration::s_BULLET_SHOT_SOUND_FILE_PATH
	);
	if (NULL == s_pSoundShot) {
		OutputDebugString(_T("BULLET_COLLISION_SOUND_FILE not found; continuing without sound.\n"));
		V_THROW(E_FAIL);
	}
}


/**
 * @brief Direct3D device �̉�̌�ɌĂ΂��
 *
 * onDeviceCreated() �Ŏ擾�������\�[�X�������ŉ������
 *
 */
void Bullet::onDeviceDestroyed()
{
	std::for_each(s_meshTexturePointers.begin(), s_meshTexturePointers.end(), acut::Release<LPDIRECT3DTEXTURE9>());
	s_meshTexturePointers.clear();
	SAFE_RELEASE(s_pMesh);
	SAFE_DELETE(s_pSoundShot);

#ifdef DEBUG
	s_debugCollision.finalize();
#endif
}


/**
 * @brief �����Ă�������ֈړ�����
 *
 */
void Bullet::act(const float elapsedTime)
{
	if (!this->isExist()) {
		return;	
	}

	m_previousPosition = m_position;
	m_existenceTime += elapsedTime;
	D3DXVECTOR3 normalVector;
	D3DXVec3Normalize(&normalVector, &m_direction);
	m_position += normalVector * GameConfiguration::s_BULLET_SPEED * elapsedTime;
}


/**
 * @brief �e��`�悷��
 *
 */
void Bullet::render()
{
	if (!this->isExist()) {
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = DXUTGetD3D9Device();

	{
		// world�s��̃Z�b�g
		D3DXMATRIXA16 worldMatrix;
		V_THROW(pDevice->SetTransform(
			D3DTS_WORLD,
			D3DXMatrixTranslation(&worldMatrix, m_position.x, m_position.y, m_position.z)
		));
	}

	for (DWORD i = 0; i < s_numberOfMaterials; ++i) {
		// �}�e���A���ƃe�N�X�`�����Z�b�g
		V_THROW(pDevice->SetMaterial(&s_meshMaterials.at(i)));
		V_THROW(pDevice->SetTexture(0, s_meshTexturePointers.at(i)));

		// mesh�`��
		V_THROW(s_pMesh->DrawSubset(i));
	}

#ifdef DEBUG
	s_debugCollision.render();
#endif
}


/**
 * @brief �������Ԃ͈͓̔������ׂ�
 *
 * @return �������ԓ�: true�A�������ԊO: false
 * 
 */
bool Bullet::isExist() const
{
	if (GameConfiguration::s_BULLET_DISAPPEARANCE_TIME < m_existenceTime) {
		return false;
	}
	return true;
}


/**
 * @brief �e�𔭎˂�����Ԃɂ���
 *
 */
void Bullet::shot(const D3DXVECTOR3& position, const D3DXVECTOR3& direction)
{
	// �e���E�肩�甭�˂��Ă���悤�Ɍ����邽�߁A
	// �v���C���[�ɑ΂��Ẳ��ʒu�𒲐�����
	// �v���C���[�ɑ΂��ẲE�����̃x�N�g�����v�Z
	D3DXVECTOR3 vectorSide;
	D3DXVec3Cross(&vectorSide, &D3DXVECTOR3(0.f, 1.f, 0.f), &direction);
	D3DXVec3Normalize(&vectorSide, &vectorSide);
	// ���ˈʒu�̐ݒ�
	m_position = position;
	m_position.x += vectorSide.x * 0.08f;
	m_position.y += GameConfiguration::s_BULLET_OFFSET_POSITION_Y;
	m_position.z += vectorSide.z * 0.08f;

	// ���˕����̐ݒ�
	m_direction = direction;

	// ��ʂɑ��݂��鎞�Ԃ̏�����
	m_existenceTime = 0.0f;

	// shot���̌��ʉ�
	V_THROW(s_pSoundShot->Reset());
	V_THROW(s_pSoundShot->Play(0, 0, -1000));
}


/**
 * @brief �Փˎ��̏���
 *
 */
void Bullet::collide()
{
	DEBUG_OUTPUTF("%s : \n", __FUNCTIONW__);
	m_existenceTime = GameConfiguration::s_BULLET_DISAPPEARANCE_TIME;
}


/**
 * @brief �ʒu���Z�b�g
 *
 * @param position �ʒu
 *
 */
void Bullet::setPosition(const D3DXVECTOR3& position)
{
	m_position = position;
}


/**
 * @brief �ʒu���擾
 *
 * @return �ʒu
 *
 */
D3DXVECTOR3 Bullet::getPosition() const
{
	return m_position;
}


/**
 * @brief �O�̃t���[�����̈ʒu���擾
 *
 * @return �O�̃t���[�����̈ʒu
 *
 */
D3DXVECTOR3 Bullet::getPreviousPosition() const
{
	return m_previousPosition;
}

