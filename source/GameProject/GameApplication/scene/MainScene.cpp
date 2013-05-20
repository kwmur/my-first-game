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
	// 特に処理は無し
}


/**
 * @brief destructor
 *
 */
MainScene::~MainScene()
{
	// 特に処理は無し
}


/**
 * @brief ゲームの初期化処理
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
	m_Camera.SetEnableYAxisMovement(false); // カメラのクレーンは無し
}


/**
 * @brief soundの初期化
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
		MessageBox(NULL, _T("BGMファイルの読み込みでエラーが発生しました。"), _T("エラー発生"), MB_ICONEXCLAMATION);
		throw;
	}
}


/**
 * @brief シーンのゲーム的な初期化
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
	// std::vector::resize() は、インスタンスを1つ生成した後、
	// 1つ生成したインスタンスをresizeの引数に渡されたサイズ分コピーしている。
	// m_positionなどのデータもコピーされてしまう為、同じ位置に出現することになる。
	// その為、generate()を呼び出して生成位置を再設定する。
	std::for_each(m_enemy.begin(), m_enemy.end(), std::mem_fun_ref(&Enemy::generate));


	Bullet::initialize();
	Bullet::s_lastShotTime= GameApplication::s_lastAnimTime = DXUTGetGlobalTimer()->GetTime();
	// 弾を表示しないようにする為、衝突したことにする
	std::for_each(m_bullets.begin(), m_bullets.end(), std::mem_fun_ref(&Bullet::collide));
}


/**
 * @brief ゲームの終了処理
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


	// 床の初期化
	m_floor.onDeviceCreated();	

	// プレイヤーの初期化
	m_player.onDeviceCreated();

	// 敵の初期化と生成
	Enemy::onDeviceCreated();
	//m_enemy.resize(GameConfiguration::s_ENEMY_MAX);

	// 弾の生成
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
 * (ここで作ったResourcesはOnLostDeviceでリリースしないといけない。）
 * この関数は一番最初に呼ばれるっぽい。
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

    // lighting の設定
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
 * この関数は毎フレーム呼ばれる。
 * 
 * @param fTime アプリケーションが開始されてからの経過時間
 * @param fElapsedTime 最後のフレームからの経過時間
 * @param pUserConatext コールバック関数に渡すユーザー定義の値
 *
 */
void MainScene::onFrameMove(double fTime, float fElapsedTime, void* pUserContext)
{
    // Update the camera's position based on user input
	// カメラの位置更新
	// ユーザーの入力をもとにする
    m_Camera.FrameMove(fElapsedTime);
	//const D3DXVECTOR3* pEyePt = m_Camera.GetEyePt();
	//DEBUG_OUTPUTF("m_Camera.GetEyePt() : x = %f, y = %f, z = %f\n", pEyePt->x, pEyePt->y, pEyePt->z);

	// プレイヤー
	m_player.act(fElapsedTime);

	// 敵
	Enemy::setPlayerPosition(m_player.getPosition());
	std::for_each(m_enemy.begin(), m_enemy.end(), std::bind2nd(std::mem_fun_ref(&Enemy::act), fElapsedTime));

	// 弾
	// フレーム更新
	std::for_each(
		m_bullets.begin(),
		m_bullets.end(),
		std::bind2nd(std::mem_fun_ref(&Bullet::act), fElapsedTime)
	);
	// ショット
	if (Bullet::s_shot) {
		// 0.0の場合、まだショットしてない、
		// または左マウスボタンを離したことを意味する
		// TODO: 意味がわかりづらいので、わかりやすくする
		if (0.0 == Bullet::s_lastShotTime) {
			// 一個ショットできるように時間を設定する
			Bullet::s_lastShotTime = fTime - GameConfiguration::s_BULLET_FIRING_INTERVAL;
		}
		// 発射する弾の数を計算
		const int bulletCount = static_cast<int>((fTime - Bullet::s_lastShotTime) / GameConfiguration::s_BULLET_FIRING_INTERVAL);
		
		for (int i = 0; i < bulletCount; ++i) {
			// 表示時間をオーバーしている弾を探す
			Bullet::VectorIterator it = std::find_if(
				m_bullets.begin(),
				m_bullets.end(),
				boost::not1(boost::mem_fun_ref(&Bullet::isExist))
			);
			if (m_bullets.end() != it) {
				// Playerの向いている方向へ弾を撃つ
				it->shot(m_player.getPosition(), m_player.getFacing());

				// 今の時間から本来発射された時間を引く
				// それが今回のact()での移動量になる
				// 本来発射されている時間からの経過時間を渡す
				it->act(static_cast<float>(fElapsedTime - GameConfiguration::s_BULLET_FIRING_INTERVAL * i));
			}
		}

		// ショットしたら、最後に撃った時間を設定
		if (0 < bulletCount) {
			Bullet::s_lastShotTime = fTime;
		}
	}


	// 弾と敵との衝突判定
	for (Bullet::VectorIterator itBullet = m_bullets.begin(), itBulletEnd = m_bullets.end();
		itBullet != itBulletEnd;
		++itBullet
	) {
		// 弾が存在しているか
		if (itBullet->isExist()) {
			for (Enemy::VectorIterator itEnemy = m_enemy.begin(), itEnemyEnd = m_enemy.end();
				itEnemy != itEnemyEnd;
				++itEnemy
			) {
				// 敵が死んでないか
				if (!itEnemy->isDead()) {
					// 敵と弾との衝突判定
					// A: Bullet
					// B: Enemy
					FLOAT collideTime = 0.0f;
					D3DXVECTOR3 bulletCollidePosition;
					D3DXVECTOR3 enemyCollidePosition;
					if (acut::Collision::calculateParticleCollision(
						GameConfiguration::s_BULLET_COLLISION_RADIUS, // パーティクルAの半径
						GameConfiguration::s_ENEMY_COLLISION_RADIUS, // パーティクルBの半径
						&itBullet->getPreviousPosition(), // パーティクルAの前の位置
						&itBullet->getPosition(), // パーティクルAの次の到達位置
						&itEnemy->getPreviousPosition(), // パーティクルBの前位置
						&itEnemy->getPosition(), // パーティクルBの次の到達位置
						&collideTime, // 衝突時間を格納するFLOAT型へのポインタ
						&bulletCollidePosition, // パーティクルAの衝突位置を格納するD3DXVECTOR型へのポインタ
						&enemyCollidePosition // パーティクルBの衝突位置を格納するD3DXVECTOR型へのポインタ
					)) {
						// TODO: 複数の敵に当たっている場合、一番手前の敵のみヒットしたことにする

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

	// プレイヤーと敵との衝突判定
	for (Enemy::VectorIterator itEnemy = m_enemy.begin(), itEnemyEnd = m_enemy.end();
		itEnemy != itEnemyEnd;
		++itEnemy
	) {
		// 敵が死んでないか
		if (!itEnemy->isDead()) {
			// 敵と弾との衝突判定
			// A: Player
			// B: Enemy
			FLOAT collideTime = 0.0f;
			D3DXVECTOR3 playerCollidePosition;
			D3DXVECTOR3 enemyCollidePosition;
			if (acut::Collision::calculateParticleCollision(
				GameConfiguration::s_PLAYER_COLLISION_RADIUS, // パーティクルAの半径
				GameConfiguration::s_ENEMY_COLLISION_RADIUS, // パーティクルBの半径
				&m_player.getPreviousCollisionPosition(), // パーティクルAの前の位置
				&m_player.getCollisionPosition(), // パーティクルAの次の到達位置
				&itEnemy->getPreviousPosition(), // パーティクルBの前位置
				&itEnemy->getPosition(), // パーティクルBの次の到達位置
				&collideTime, // 衝突時間を格納するFLOAT型へのポインタ
				&playerCollidePosition, // パーティクルAの衝突位置を格納するD3DXVECTOR型へのポインタ
				&enemyCollidePosition // パーティクルBの衝突位置を格納するD3DXVECTOR型へのポインタ
			)) {
				m_player.collide();

				itEnemy->setPosition(enemyCollidePosition);
				itEnemy->collide();
			}
		}
	}

	// 一定時間が経過していたら、敵を新しく生成する
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
 * @param pd3dDevice デバイス
 * @param fTime アプリケーションが開始されてからの経過時間
 * @param fElapsedTime 最後のフレームからの経過時間
 * @param pUserConatext コールバック関数に渡すユーザー定義の値
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
        D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f); // カメラのロール
        playerFacing.x *= 0.85f;
		playerFacing.y *= 0.85f;
		playerFacing.z *= 0.85f;

		// cameraの位置をplayerの位置から後ろへ下げる
          cameraPosition -= playerFacing; 
		// y座標を補正
		if (cameraPosition.y <= 0.1f) {
			cameraPosition.y = 0.1f;
		}

		// 注視点をプレイヤーの位置から前 にする
          vAt += playerFacing;

		// view matrix を設定
          D3DXMatrixLookAtLH(&viewMatrix, &cameraPosition, &vAt, &vUp);
          V(pd3dDevice->SetTransform(D3DTS_VIEW, &viewMatrix));

		// projection matrix を設定
          const D3DSURFACE_DESC* pBackBufferSurfaceDesc = DXUTGetD3D9BackBufferSurfaceDesc();
          float fAspectRatio = pBackBufferSurfaceDesc->Width / (FLOAT)pBackBufferSurfaceDesc->Height;
          D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DX_PI / 3, fAspectRatio, 0.1f, 100.0f);
          V(pd3dDevice->SetTransform(D3DTS_PROJECTION, &projectionMatrix));

		// ライトの向きを注視点からカメラの位置へ向け、正規化する
          D3DXVec3Subtract(&lightDirectionVector, &cameraPosition, &vAt);
          D3DXVec3Normalize(&lightDirectionVector, &lightDirectionVector);

// Playerのテスト
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

	// scoreの表示
	acut::tstringstream ssScore;
	ssScore << _T("SCORE : ") << GameApplication::s_score << std::endl;
	txtHelper.DrawTextLine(ssScore.str().c_str());

	// lifeの表示
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
	// 左クリックされていたら、ショットを撃つ
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
