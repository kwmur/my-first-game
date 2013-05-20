#ifndef ACUT_GAME_CONFIGURATION_H__
#define ACUT_GAME_CONFIGURATION_H__



class GameConfiguration {
public:

	// Game
	static const acut::TString s_GAME_TITLE; //!< ゲームタイトル
	static const int s_BACK_BUFFER_WIDTH; //!< バックバッファの横幅
	static const int s_BACK_BUFFER_HEIGHT; //!< バックバッファの縦幅

	// TitleScene
	static const acut::TString s_TITLE_SCENE_BGM_FILE_PATH; //!< タイトルシーンのBGMファイルへのパス
	static const acut::TString s_TITLE_SCENE_IMAGE_FILE_PATH; //!< タイトルシーンの画像ファイルへのパス

	// MainScene
	static const acut::TString s_MAIN_SCENE_BGM_FILE_PATH; //!< メインシーンのBGMファイルへのパス

	// ResultScene
	static const acut::TString s_RESULT_SCENE_BGM_FILE_PATH; //!< リザルトシーンのBGMファイルへのパス
	static const acut::TString s_RESULT_SCENE_IMAGE_FILE_PATH; //!< リザルトシーンの画像ファイルへのパス

	// Floor
	static const float s_FLOOR_MOVE_RADIUS; //!< 床の移動可能半径
	// mesh
	static const acut::TString s_FLOOR_MESH_FOLDER_PATH; //!< 床のメッシュファイルが格納されているフォルダパス
	static const acut::TString s_FLOOR_MESH_FILE_PATH; //!< のメッシュファイルのパス

	// Bullet
	static const float s_BULLET_SPEED; //!< 弾の移動速度
	static const float s_BULLET_DISAPPEARANCE_TIME; //!< 弾の消滅する時間
	static const double s_BULLET_FIRING_INTERVAL; //!< 弾の発射間隔時間
	static const float s_BULLET_OFFSET_POSITION_Y; //!< 弾のY軸初期位置の埋め合わせ
	static const size_t s_BULLET_MAX; //!< 弾の最大表示数
	// collision
	static const float s_BULLET_COLLISION_RADIUS; //!< 弾の衝突半径
	// mesh
	static const acut::TString s_BULLET_MESH_FOLDER_PATH; //!< 弾のメッシュファイルが格納されているフォルダパス
	static const acut::TString s_BULLET_MESH_FILE_PATH; //!< 弾のメッシュファイルのパス
	// sound
	static const acut::TString s_BULLET_SHOT_SOUND_FILE_PATH; //!< 発射時の効果音ファイルへのパス

	// Enemy
	static const float s_ENEMY_SPEED; //!< 敵の移動速度
	static const int s_ENEMY_HEALTH; //!< 敵の体力
	static const float s_ENEMY_DIE_OUT_TIME; //!< 敵のhealthが0になってから消滅するまでの時間
	static const float s_ENEMY_GENERATE_INTERVAL; //!< 敵の生成間隔時間
	static const size_t s_ENEMY_MAX; //!< 敵の最大表示数
	static const float s_ENEMY_GENERATE_DISTANCE; //!< 生成される位置がプレイヤーと一定の距離をとっているか
	static const float GameConfiguration::s_ENEMY_GENERATE_POSITION_Y_1; //!< 生成時の位置 生成1回目
	static const float GameConfiguration::s_ENEMY_GENERATE_POSITION_Y_2; //!< 生成時の位置 生成2回目以降
	static const int GameConfiguration::s_ENEMY_GENERATE_RANDOM_DIVISION; //!< 生成時に乱数を割る値
	static const int GameConfiguration::s_ENEMY_GENERATE_RANDOM_SUBTRACTION; //!< 生成時に乱数から引く値
	// collision
	static const float s_ENEMY_COLLISION_RADIUS; //!< 敵の衝突半径
	// mesh
	static const acut::TString s_ENEMY_MESH_FOLDER_PATH; //!< 敵のメッシュファイルが格納されているフォルダパス
	static const acut::TString s_ENEMY_MESH_FILE_PATH; //!< 敵のメッシュファイルのパス
	// sound
	static const acut::TString s_ENEMY_COLLISION_SOUND_FILE_PATH; //!< 衝突時の効果音ファイルへのパス

	// Player
	static const int s_PLAYER_HEALTH; //!< プレイヤーの体力
	static const float s_PLAYER_SPEED; //!< プレイヤーの移動速度
	static const float s_PLAYER_TURN_SPEED; //!< プレイヤーの向きを変える速度
	//static const float GameConfiguration::s_PLAYER_COLLIDE_CHANGE_COLOR_TIME; //!< プレイヤーが敵と衝突時に色が変わる時間
	static const D3DXVECTOR3 s_PLAYER_DEFAULT_POSITION; //!< プレイヤーのデフォルトの位置
	static const D3DXVECTOR3 s_PLAYER_DEFAULT_SCALE; //!< プレイヤーのデフォルトの縮尺
	static const D3DXVECTOR3 s_PLAYER_DEFAULT_ROTATION; //!< プレイヤーのデフォルトの向き
	// collision
	static const float s_PLAYER_COLLISION_RADIUS; //!< プレイヤーの衝突半径
	// mesh
	//static const acut::TString s_PLAYER_MESH_FOLDER_PATH; //!< プレイヤーのメッシュファイルが格納されているフォルダパス
	static const acut::TString s_PLAYER_MESH_FILE_PATH; //!< プレイヤーのメッシュファイルのパス
	// sound
	static const acut::TString s_PLAYER_COLLISION_SOUND_FILE_PATH; //!< 衝突時の効果音ファイルへのパス

private:
	// インスタンス化未対応
	GameConfiguration();
	~GameConfiguration();
};



#endif // #ifndef ACUT_GAME_CONFIGURATION_H__
