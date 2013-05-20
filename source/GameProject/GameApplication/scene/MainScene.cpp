#include "MainScene.h"
#include "../GameConfiguration.h"
#include "../GameApplication.h"



/**
 * @brief constructor
 *
 */
MainScene::MainScene() :
	m_bullets(GameConfiguration::s_BULLET_MAX),
	m_enemy(GameConfiguration::s_ENEMY_MAX)
{
	// ���ɏ����͖���
}


/**
 * @brief destructor
 *
 */
MainScene::~MainScene()
{
	// ���ɏ����͖���
}


/**
 * @brief �Q�[���̏���������
 *
 */
void MainScene::initialize()
{
	m_pFont = 0;
	m_pTextSprite = 0;
	m_pEffect = 0;
	GameApplication::s_bPlaySounds = true; // whether to play sounds
	GameApplication::s_lastAnimTime = 0.0; // Time for the animations

    // Initialize dialogs
    m_SettingsDlg.Init(&m_DialogResourceManager);
    m_HUD.Init(&m_DialogResourceManager);
    m_SampleUI.Init(&m_DialogResourceManager);

    // Add mixed vp to the available vp choices in device settings dialog.
    DXUTGetD3D9Enumeration()->SetPossibleVertexProcessingList( true, false, false, true );

    // Setup the camera with view matrix
    //D3DXVECTOR3 cameraPosition( .5f, .55f, -.2f );
    D3DXVECTOR3 cameraPosition( .5f, .35f, -.2f );
    D3DXVECTOR3 vAt( .5f,  .125f, .5f );
    m_Camera.SetViewParams( &cameraPosition, &vAt );
    m_Camera.SetScalers( 0.01f, 1.0f );  // Camera movement parameters
	m_Camera.SetEnableYAxisMovement(false); // �J�����̃N���[���͖���
}


/**
 * @brief sound�̏�����
 *
 */
void MainScene::initializeSound(CSoundManager* pSoundManager)
{
	try {
		this->loadBgm(
			pSoundManager,
			GameConfiguration::s_MAIN_SCENE_BGM_FILE_PATH
		);
	} catch (acut::Exception&) {
		MessageBox(NULL, _T("BGM�t�@�C���̓ǂݍ��݂ŃG���[���������܂����B"), _T("�G���[����"), MB_ICONEXCLAMATION);
		throw;
	}
}


/**
 * @brief �V�[���̃Q�[���I�ȏ�����
 *
 */
void MainScene::initializeGameScene()
{
	GameApplication::s_score = 0;

	m_player.initialize(
		GameConfiguration::s_PLAYER_DEFAULT_POSITION,
		GameConfiguration::s_PLAYER_DEFAULT_SCALE,
		GameConfiguration::s_PLAYER_DEFAULT_ROTATION
	);

	Enemy::setPlayerPosition(m_player.getPosition());
	// std::vector::resize() �́A�C���X�^���X��1����������A
	// 1���������C���X�^���X��resize�̈����ɓn���ꂽ�T�C�Y���R�s�[���Ă���B
	// m_position�Ȃǂ̃f�[�^���R�s�[����Ă��܂��ׁA�����ʒu�ɏo�����邱�ƂɂȂ�B
	// ���ׁ̈Agenerate()���Ăяo���Đ����ʒu���Đݒ肷��B
	std::for_each(m_enemy.begin(), m_enemy.end(), std::mem_fun_ref(&Enemy::generate));


	Bullet::initialize();
	Bullet::s_lastShotTime= GameApplication::s_lastAnimTime = DXUTGetGlobalTimer()->GetTime();
	// �e��\�����Ȃ��悤�ɂ���ׁA�Փ˂������Ƃɂ���
	std::for_each(m_bullets.begin(), m_bullets.end(), std::mem_fun_ref(&Bullet::collide));
}


/**
 * @brief �Q�[���̏I������
 *
 */
void MainScene::finalize()
{
	AbstractScene::finalize();
	Bullet::finalize();
	Enemy::finalize();
	m_player.finalize();
}


/**
 * @brief
 *
 * This callback function will be called immediately after the Direct3D device has been
 * created, which will happen during application initialization and windowed/full screen
 * toggles. This is the best location to create D3DPOOL_MANAGED resources since these
 * resources need to be reloaded whenever the device is destroyed. Resources created
 * here should be released in the OnDestroyDevice callback.
 *
 */
HRESULT MainScene::onDeviceCreated(
	IDirect3DDevice9* pd3dDevice,
	const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext
)
{
	HRESULT hr;

	V_RETURN(m_DialogResourceManager.OnD3D9CreateDevice(pd3dDevice));
	V_RETURN(m_SettingsDlg.OnD3D9CreateDevice(pd3dDevice));
	// Initialize the font
	V_RETURN(D3DXCreateFont(
		pd3dDevice,
		15,
		0,
		FW_BOLD,
		1,
		FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"Arial",
		&m_pFont
	));


	// ���̏�����
	m_floor.onDeviceCreated();	

	// �v���C���[�̏�����
	m_player.onDeviceCreated();

	// �G�̏������Ɛ���
	Enemy::onDeviceCreated();
	//m_enemy.resize(GameConfiguration::s_ENEMY_MAX);

	// �e�̐���
	Bullet::onDeviceCreated();



    // Define DEBUG_VS and/or DEBUG_PS to debug vertex and/or pixel shaders with the
    // shader debugger. Debugging vertex shaders requires either REF or software vertex
    // processing, and debugging pixel shaders requires REF.  The
    // D3DXSHADER_FORCE_*_SOFTWARE_NOOPT flag improves the debug experience in the
    // shader debugger.  It enables source level debugging, prevents instruction
    // reordering, prevents dead code elimination, and forces the compiler to compile
    // against the next higher available software target, which ensures that the
    // unoptimized shaders do not exceed the shader model limitations.  Setting these
    // flags will cause slower rendering since the shaders will be unoptimized and
    // forced into software.  See the DirectX documentation for more information about
    // using the shader debugger.
    DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE;

#if defined( DEBUG ) || defined( _DEBUG )
    // Set the D3DXSHADER_DEBUG flag to embed debug information in the shaders.
    // Setting this flag improves the shader debugging experience, but still allows 
    // the shaders to be optimized and to run exactly the way they will run in 
    // the release configuration of this program.
    dwShaderFlags |= D3DXSHADER_DEBUG;
#endif

#ifdef DEBUG_VS
        dwShaderFlags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
#endif
#ifdef DEBUG_PS
        dwShaderFlags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
#endif


//    WCHAR filePath[MAX_PATH];

    // Read the D3DX effect file
//    V_RETURN(DXUTFindDXSDKMediaFileCch(filePath, MAX_PATH, L"media\\shader\\MultiAnimation.fx"));

    // If this fails, there should be debug output as to
    // they the .fx file failed to compile
//    V_RETURN(D3DXCreateEffectFromFile(pd3dDevice, filePath, NULL, NULL, dwShaderFlags, NULL, &m_pEffect, NULL));
//    m_pEffect->SetTechnique("RenderScene");

	return S_OK;
}


/**
 * @brief
 * This callback function will be called immediately after the Direct3D device has been
 * reset, which will happen after a lost device scenario. This is the best location to
 * create D3DPOOL_DEFAULT resources since these resources need to be reloaded whenever
 * the device is lost. 
 * Resources created here should be released in the OnLostDevice callback.
 * (�����ō����Resources��OnLostDevice�Ń����[�X���Ȃ��Ƃ����Ȃ��B�j
 * ���̊֐��͈�ԍŏ��ɌĂ΂����ۂ��B
 *
 * @param
 * @param
 * @param
 * @return
 */
HRESULT MainScene::onDeviceReset(
	IDirect3DDevice9* pd3dDevice,
	const D3DSURFACE_DESC* pBackBufferSurfaceDesc,
	void* pUserContext
)
{
    HRESULT hr;

    V_RETURN(m_DialogResourceManager.OnD3D9ResetDevice());
    V_RETURN(m_SettingsDlg.OnD3D9ResetDevice());

    // get device caps
    D3DCAPS9 caps;
    pd3dDevice->GetDeviceCaps(&caps);

    if( m_pFont )
        V_RETURN( m_pFont->OnResetDevice() );
    if( m_pEffect )
        V_RETURN( m_pEffect->OnResetDevice() );

    // Create a sprite to help batch calls when drawing many lines of text
    V_RETURN( D3DXCreateSprite( pd3dDevice, &m_pTextSprite ) );

    // Setup the camera's projection parameters
    float fAspectRatio = pBackBufferSurfaceDesc->Width / ( FLOAT )pBackBufferSurfaceDesc->Height;
    m_Camera.SetProjParams( D3DX_PI / 3, fAspectRatio, 0.001f, 100.0f );

    // lighting �̐ݒ�
    pd3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
    pd3dDevice->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_ARGB(255, 200, 200, 255));
    pd3dDevice->LightEnable(0, TRUE);
    pd3dDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);


	// reset the timer
	GameApplication::s_lastAnimTime = DXUTGetGlobalTimer()->GetTime();

	// Adjust the dialog parameters.
	m_HUD.SetLocation( pBackBufferSurfaceDesc->Width - 170, 0 );
	m_HUD.SetSize( 170, 170 );
	m_SampleUI.SetLocation( pBackBufferSurfaceDesc->Width - 170, pBackBufferSurfaceDesc->Height - 270 );
	m_SampleUI.SetSize( 170, 220 );



	return S_OK;
}


/**
 * @brief
 * This callback function will be called once at the beginning of every frame. This is the
 * best location for your application to handle updates to the scene, but is not
 * intended to contain actual rendering calls, which should instead be placed in the
 * OnFrameRender callback.
 * 
 * ���̊֐��͖��t���[���Ă΂��B
 * 
 * @param fTime �A�v���P�[�V�������J�n����Ă���̌o�ߎ���
 * @param fElapsedTime �Ō�̃t���[������̌o�ߎ���
 * @param pUserConatext �R�[���o�b�N�֐��ɓn�����[�U�[��`�̒l
 *
 */
void MainScene::onFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
    // Update the camera's position based on user input
	// �J�����̈ʒu�X�V
	// ���[�U�[�̓��͂����Ƃɂ���
    m_Camera.FrameMove(fElapsedTime);
	//const D3DXVECTOR3* pEyePt = m_Camera.GetEyePt();
	//DEBUG_OUTPUTF("m_Camera.GetEyePt() : x = %f, y = %f, z = %f\n", pEyePt->x, pEyePt->y, pEyePt->z);

	// �v���C���[
	m_player.act(fElapsedTime);

	// �G
	Enemy::setPlayerPosition(m_player.getPosition());
	std::for_each(m_enemy.begin(), m_enemy.end(), std::bind2nd(std::mem_fun_ref(&Enemy::act), fElapsedTime));

	// �e
	// �t���[���X�V
	std::for_each(
		m_bullets.begin(),
		m_bullets.end(),
		std::bind2nd(std::mem_fun_ref(&Bullet::act), fElapsedTime)
	);
	// �V���b�g
	if (Bullet::s_shot) {
		// 0.0�̏ꍇ�A�܂��V���b�g���ĂȂ��A
		// �܂��͍��}�E�X�{�^���𗣂������Ƃ��Ӗ�����
		// TODO: �Ӗ����킩��Â炢�̂ŁA�킩��₷������
		if (0.0 == Bullet::s_lastShotTime) {
			// ��V���b�g�ł���悤�Ɏ��Ԃ�ݒ肷��
			Bullet::s_lastShotTime = fTime - GameConfiguration::s_BULLET_FIRING_INTERVAL;
		}
		// ���˂���e�̐����v�Z
		const int bulletCount = static_cast<int>((fTime - Bullet::s_lastShotTime) / GameConfiguration::s_BULLET_FIRING_INTERVAL);
		
		for (int i = 0; i < bulletCount; ++i) {
			// �\�����Ԃ��I�[�o�[���Ă���e��T��
			Bullet::VectorIterator it = std::find_if(
				m_bullets.begin(),
				m_bullets.end(),
				boost::not1(boost::mem_fun_ref(&Bullet::isExist))
			);
			if (m_bullets.end() != it) {
				// Player�̌����Ă�������֒e������
				it->shot(m_player.getPosition(), m_player.getFacing());

				// ���̎��Ԃ���{�����˂��ꂽ���Ԃ�����
				// ���ꂪ�����act()�ł̈ړ��ʂɂȂ�
				// �{�����˂���Ă��鎞�Ԃ���̌o�ߎ��Ԃ�n��
				it->act(static_cast<float>(fElapsedTime - GameConfiguration::s_BULLET_FIRING_INTERVAL * i));
			}
		}

		// �V���b�g������A�Ō�Ɍ��������Ԃ�ݒ�
		if (0 < bulletCount) {
			Bullet::s_lastShotTime = fTime;
		}
	}


	// �e�ƓG�Ƃ̏Փ˔���
	for (Bullet::VectorIterator itBullet = m_bullets.begin(), itBulletEnd = m_bullets.end();
		itBullet != itBulletEnd;
		++itBullet
	) {
		// �e�����݂��Ă��邩
		if (itBullet->isExist()) {
			for (Enemy::VectorIterator itEnemy = m_enemy.begin(), itEnemyEnd = m_enemy.end();
				itEnemy != itEnemyEnd;
				++itEnemy
			) {
				// �G������łȂ���
				if (!itEnemy->isDead()) {
					// �G�ƒe�Ƃ̏Փ˔���
					// A: Bullet
					// B: Enemy
					FLOAT collideTime = 0.0f;
					D3DXVECTOR3 bulletCollidePosition;
					D3DXVECTOR3 enemyCollidePosition;
					if (acut::Collision::calculateParticleCollision(
						GameConfiguration::s_BULLET_COLLISION_RADIUS, // �p�[�e�B�N��A�̔��a
						GameConfiguration::s_ENEMY_COLLISION_RADIUS, // �p�[�e�B�N��B�̔��a
						&itBullet->getPreviousPosition(), // �p�[�e�B�N��A�̑O�̈ʒu
						&itBullet->getPosition(), // �p�[�e�B�N��A�̎��̓��B�ʒu
						&itEnemy->getPreviousPosition(), // �p�[�e�B�N��B�̑O�ʒu
						&itEnemy->getPosition(), // �p�[�e�B�N��B�̎��̓��B�ʒu
						&collideTime, // �Փˎ��Ԃ��i�[����FLOAT�^�ւ̃|�C���^
						&bulletCollidePosition, // �p�[�e�B�N��A�̏Փˈʒu���i�[����D3DXVECTOR�^�ւ̃|�C���^
						&enemyCollidePosition // �p�[�e�B�N��B�̏Փˈʒu���i�[����D3DXVECTOR�^�ւ̃|�C���^
					)) {
						// TODO: �����̓G�ɓ������Ă���ꍇ�A��Ԏ�O�̓G�̂݃q�b�g�������Ƃɂ���

						itBullet->setPosition(bulletCollidePosition);
						itBullet->collide();

						itEnemy->setPosition(enemyCollidePosition);
						itEnemy->collide();

						GameApplication::addScore(100);
					}
				}
			}
		}
	}

	// �v���C���[�ƓG�Ƃ̏Փ˔���
	for (Enemy::VectorIterator itEnemy = m_enemy.begin(), itEnemyEnd = m_enemy.end();
		itEnemy != itEnemyEnd;
		++itEnemy
	) {
		// �G������łȂ���
		if (!itEnemy->isDead()) {
			// �G�ƒe�Ƃ̏Փ˔���
			// A: Player
			// B: Enemy
			FLOAT collideTime = 0.0f;
			D3DXVECTOR3 playerCollidePosition;
			D3DXVECTOR3 enemyCollidePosition;
			if (acut::Collision::calculateParticleCollision(
				GameConfiguration::s_PLAYER_COLLISION_RADIUS, // �p�[�e�B�N��A�̔��a
				GameConfiguration::s_ENEMY_COLLISION_RADIUS, // �p�[�e�B�N��B�̔��a
				&m_player.getPreviousCollisionPosition(), // �p�[�e�B�N��A�̑O�̈ʒu
				&m_player.getCollisionPosition(), // �p�[�e�B�N��A�̎��̓��B�ʒu
				&itEnemy->getPreviousPosition(), // �p�[�e�B�N��B�̑O�ʒu
				&itEnemy->getPosition(), // �p�[�e�B�N��B�̎��̓��B�ʒu
				&collideTime, // �Փˎ��Ԃ��i�[����FLOAT�^�ւ̃|�C���^
				&playerCollidePosition, // �p�[�e�B�N��A�̏Փˈʒu���i�[����D3DXVECTOR�^�ւ̃|�C���^
				&enemyCollidePosition // �p�[�e�B�N��B�̏Փˈʒu���i�[����D3DXVECTOR�^�ւ̃|�C���^
			)) {
				m_player.collide();

				itEnemy->setPosition(enemyCollidePosition);
				itEnemy->collide();
			}
		}
	}

	// ��莞�Ԃ��o�߂��Ă�����A�G��V������������
	if ((Enemy::s_generateInterval -= fElapsedTime) <= 0) {
		Enemy::VectorIterator itEnemy = std::find_if(
			m_enemy.begin(),
			m_enemy.end(),
			boost::not1(boost::mem_fun_ref(&Enemy::isExist))
		);
		if (m_enemy.end() != itEnemy) {
			itEnemy->generate();
		}
	}
}


/**
 * @brief
 * This callback function will be called at the end of every frame to perform all the
 * rendering calls for the scene, and it will also be called if the window needs to be
 * repainted. After this function has returned, DXUT will call
 * IDirect3DDevice9::Present to display the contents of the next buffer in the swap chain
 *
 * @param pd3dDevice �f�o�C�X
 * @param fTime �A�v���P�[�V�������J�n����Ă���̌o�ߎ���
 * @param fElapsedTime �Ō�̃t���[������̌o�ߎ���
 * @param pUserConatext �R�[���o�b�N�֐��ɓn�����[�U�[��`�̒l
 *
 */
void MainScene::onFrameRender(
	IDirect3DDevice9* pd3dDevice,
	double fTime, 
	float fElapsedTime,
	void* pUserContext
)
{
    // If the settings dialog is being shown, then
    // render it instead of rendering the app's scene
    if (m_SettingsDlg.IsActive()) {
        m_SettingsDlg.OnRender(fElapsedTime);
        return;
    }

    pd3dDevice->Clear(
		0L,
		NULL, 
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
        D3DCOLOR_ARGB(0, 0x3F, 0xAF, 0xFF),
		1.0f,
		0L
	);

    HRESULT hr;

    if (SUCCEEDED(pd3dDevice->BeginScene())) {

        // set up the camera
        D3DXMATRIXA16 matrix, viewMatrix, projectionMatrix;
        D3DXVECTOR3 cameraPosition;
        D3DXVECTOR3 lightDirectionVector;

        // set follow transforms
        D3DXVECTOR3 playerPosition = m_player.getPosition();
        D3DXVECTOR3 playerFacing = m_player.getFacing();
        cameraPosition = D3DXVECTOR3(playerPosition.x, 0.20f, playerPosition.z);
        //D3DXVECTOR3 vAt(playerPosition.x, 0.0125f, playerPosition.z);
        D3DXVECTOR3 vAt(playerPosition.x, playerFacing.y, playerPosition.z);
        D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f); // �J�����̃��[��
        playerFacing.x *= 0.85f;
		playerFacing.y *= 0.85f;
		playerFacing.z *= 0.85f;

		// camera�̈ʒu��player�̈ʒu������։�����
          cameraPosition -= playerFacing; 
		// y���W��␳
		if (cameraPosition.y <= 0.1f) {
			cameraPosition.y = 0.1f;
		}

		// �����_���v���C���[�̈ʒu����O �ɂ���
          vAt += playerFacing;

		// view matrix ��ݒ�
          D3DXMatrixLookAtLH(&viewMatrix, &cameraPosition, &vAt, &vUp);
          V(pd3dDevice->SetTransform(D3DTS_VIEW, &viewMatrix));

		// projection matrix ��ݒ�
          const D3DSURFACE_DESC* pBackBufferSurfaceDesc = DXUTGetD3D9BackBufferSurfaceDesc();
          float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
          D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DX_PI / 3, fAspectRatio, 0.1f, 100.0f);
          V(pd3dDevice->SetTransform(D3DTS_PROJECTION, &projectionMatrix));

		// ���C�g�̌����𒍎��_����J�����̈ʒu�֌����A���K������
          D3DXVec3Subtract(&lightDirectionVector, &cameraPosition, &vAt);
          D3DXVec3Normalize(&lightDirectionVector, &lightDirectionVector);

// Player�̃e�X�g
		m_player.render(fElapsedTime);	

		std::for_each(m_enemy.begin(), m_enemy.end(), std::mem_fun_ref(&Enemy::render));

		std::for_each(m_bullets.begin(), m_bullets.end(), std::mem_fun_ref(&Bullet::render));

        // set the fixed function shader for drawing the floor
        // Draw the floor
		m_floor.render();

		// Output text information
		this->renderText();

		V(m_HUD.OnRender(fElapsedTime));
		V(m_SampleUI.OnRender(fElapsedTime));

		V_THROW(pd3dDevice->EndScene());
	}
}


// Render the help and statistics text. This function uses the ID3DXFont interface for
// efficient text rendering.
void MainScene::renderText()
{
    // The helper object simply helps keep track of text position, and color
    // and then it calls pFont->DrawText( m_pSprite, strMsg, -1, &rc, DT_NOCLIP, m_clr );
    // If NULL is passed in as the sprite object, then it will work however the
    // pFont->DrawText() will not be batched together.  Batching calls will improves performance.
    CDXUTTextHelper txtHelper( m_pFont, m_pTextSprite, 15 );
    const D3DSURFACE_DESC* pd3dsdBackBuffer = DXUTGetD3D9BackBufferSurfaceDesc();

    // Output statistics
    txtHelper.Begin();
    txtHelper.SetInsertionPos( 5, 5 );
    txtHelper.SetForegroundColor( D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ) );
    txtHelper.DrawTextLine( DXUTGetFrameStats( DXUTIsVsyncEnabled() ) );
    txtHelper.DrawTextLine( DXUTGetDeviceStats() );

    txtHelper.SetInsertionPos(5, 170);

	// score�̕\��
	acut::tstringstream ssScore;
	ssScore << _T("SCORE : ") << GameApplication::s_score << std::endl;
	txtHelper.DrawTextLine(ssScore.str().c_str());

	// life�̕\��
	acut::tstringstream ssLife;
	ssLife << _T("LIFE : ") << m_player.getHealth() << _T(" / ") << GameConfiguration::s_PLAYER_HEALTH << std::endl;
	txtHelper.DrawTextLine(ssLife.str().c_str());

    txtHelper.End();
}


// This callback function will be called immediately after the Direct3D device has
// entered a lost state and before IDirect3DDevice9::Reset is called. Resources created
// in the OnResetDevice callback should be released here, which generally includes all
// D3DPOOL_DEFAULT resources. See the "Lost Devices" section of the documentation for
// information about lost devices.
void MainScene::onDeviceLost(void* pUserContext)
{
	m_DialogResourceManager.OnD3D9LostDevice();
	m_SettingsDlg.OnD3D9LostDevice();
	if (m_pFont) {
		m_pFont->OnLostDevice();
	}
	if (m_pEffect) {
		m_pEffect->OnLostDevice();
	}

	SAFE_RELEASE(m_pTextSprite);
}


// This callback function will be called immediately after the Direct3D device has
// been destroyed, which generally happens as a result of application termination or
// windowed/full screen toggles. Resources created in the OnCreateDevice callback
// should be released here, which generally includes all D3DPOOL_MANAGED resources.
void MainScene::onDeviceDestroyed(void* pUserContext)
{
	m_DialogResourceManager.OnD3D9DestroyDevice();
	m_SettingsDlg.OnD3D9DestroyDevice();
	SAFE_RELEASE(m_pEffect);
	SAFE_RELEASE(m_pFont);

	m_floor.onDeviceDestroyed();
	m_player.onDeviceDestroyed();
	Enemy::onDeviceDestroyed();
	Bullet::onDeviceDestroyed();
}


// Before handling window messages, DXUT passes incoming windows
// messages to the application through this callback function. If the application sets
// *pbNoFurtherProcessing to TRUE, then DXUT will not process this message.
LRESULT MainScene::msgProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing,
                          void* pUserContext )
{
	// ���N���b�N����Ă�����A�V���b�g������
	if (WM_LBUTTONDOWN == uMsg) {
		if (!Bullet::s_shot) {
			Bullet::s_lastShotTime = 0.0;
		}
		Bullet::s_shot = true;
	}
	else if (WM_LBUTTONUP == uMsg) {
		Bullet::s_lastShotTime = 0.0;
		Bullet::s_shot = false;
	}

	if (WM_RBUTTONDOWN == uMsg) {
		static int index = 1;
		if (index == 1)
			index = 2;
		else 
			index = 1;

		m_player.changeAnimation(index);
	}

    return 0;
}

// As a convenience, DXUT inspects the incoming windows messages for
// keystroke messages and decodes the message parameters to pass relevant keyboard
// messages to the application.  The framework does not remove the underlying keystroke
// messages, which are still passed to the application's MsgProc callback.
void MainScene::keyboardProc( UINT nChar, bool bKeyDown, bool bAltDown, void* pUserContext )
{
}


// Handles the GUI events
void MainScene::onGUIEvent( UINT nEvent, int nControlID, CDXUTControl* pControl, void* pUserContext )
{
}
