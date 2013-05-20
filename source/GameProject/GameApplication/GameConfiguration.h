#ifndef ACUT_GAME_CONFIGURATION_H__
#define ACUT_GAME_CONFIGURATION_H__



class GameConfiguration {
public:

	// Game
	static const acut::TString s_GAME_TITLE; //!< �Q�[���^�C�g��
	static const int s_BACK_BUFFER_WIDTH; //!< �o�b�N�o�b�t�@�̉���
	static const int s_BACK_BUFFER_HEIGHT; //!< �o�b�N�o�b�t�@�̏c��

	// TitleScene
	static const acut::TString s_TITLE_SCENE_BGM_FILE_PATH; //!< �^�C�g���V�[����BGM�t�@�C���ւ̃p�X
	static const acut::TString s_TITLE_SCENE_IMAGE_FILE_PATH; //!< �^�C�g���V�[���̉摜�t�@�C���ւ̃p�X

	// MainScene
	static const acut::TString s_MAIN_SCENE_BGM_FILE_PATH; //!< ���C���V�[����BGM�t�@�C���ւ̃p�X

	// ResultScene
	static const acut::TString s_RESULT_SCENE_BGM_FILE_PATH; //!< ���U���g�V�[����BGM�t�@�C���ւ̃p�X
	static const acut::TString s_RESULT_SCENE_IMAGE_FILE_PATH; //!< ���U���g�V�[���̉摜�t�@�C���ւ̃p�X

	// Floor
	static const float s_FLOOR_MOVE_RADIUS; //!< ���̈ړ��\���a
	// mesh
	static const acut::TString s_FLOOR_MESH_FOLDER_PATH; //!< ���̃��b�V���t�@�C�����i�[����Ă���t�H���_�p�X
	static const acut::TString s_FLOOR_MESH_FILE_PATH; //!< �̃��b�V���t�@�C���̃p�X

	// Bullet
	static const float s_BULLET_SPEED; //!< �e�̈ړ����x
	static const float s_BULLET_DISAPPEARANCE_TIME; //!< �e�̏��ł��鎞��
	static const double s_BULLET_FIRING_INTERVAL; //!< �e�̔��ˊԊu����
	static const float s_BULLET_OFFSET_POSITION_Y; //!< �e��Y�������ʒu�̖��ߍ��킹
	static const size_t s_BULLET_MAX; //!< �e�̍ő�\����
	// collision
	static const float s_BULLET_COLLISION_RADIUS; //!< �e�̏Փ˔��a
	// mesh
	static const acut::TString s_BULLET_MESH_FOLDER_PATH; //!< �e�̃��b�V���t�@�C�����i�[����Ă���t�H���_�p�X
	static const acut::TString s_BULLET_MESH_FILE_PATH; //!< �e�̃��b�V���t�@�C���̃p�X
	// sound
	static const acut::TString s_BULLET_SHOT_SOUND_FILE_PATH; //!< ���ˎ��̌��ʉ��t�@�C���ւ̃p�X

	// Enemy
	static const float s_ENEMY_SPEED; //!< �G�̈ړ����x
	static const int s_ENEMY_HEALTH; //!< �G�̗̑�
	static const float s_ENEMY_DIE_OUT_TIME; //!< �G��health��0�ɂȂ��Ă�����ł���܂ł̎���
	static const float s_ENEMY_GENERATE_INTERVAL; //!< �G�̐����Ԋu����
	static const size_t s_ENEMY_MAX; //!< �G�̍ő�\����
	static const float s_ENEMY_GENERATE_DISTANCE; //!< ���������ʒu���v���C���[�ƈ��̋������Ƃ��Ă��邩
	static const float GameConfiguration::s_ENEMY_GENERATE_POSITION_Y_1; //!< �������̈ʒu ����1���
	static const float GameConfiguration::s_ENEMY_GENERATE_POSITION_Y_2; //!< �������̈ʒu ����2��ڈȍ~
	static const int GameConfiguration::s_ENEMY_GENERATE_RANDOM_DIVISION; //!< �������ɗ���������l
	static const int GameConfiguration::s_ENEMY_GENERATE_RANDOM_SUBTRACTION; //!< �������ɗ�����������l
	// collision
	static const float s_ENEMY_COLLISION_RADIUS; //!< �G�̏Փ˔��a
	// mesh
	static const acut::TString s_ENEMY_MESH_FOLDER_PATH; //!< �G�̃��b�V���t�@�C�����i�[����Ă���t�H���_�p�X
	static const acut::TString s_ENEMY_MESH_FILE_PATH; //!< �G�̃��b�V���t�@�C���̃p�X
	// sound
	static const acut::TString s_ENEMY_COLLISION_SOUND_FILE_PATH; //!< �Փˎ��̌��ʉ��t�@�C���ւ̃p�X

	// Player
	static const int s_PLAYER_HEALTH; //!< �v���C���[�̗̑�
	static const float s_PLAYER_SPEED; //!< �v���C���[�̈ړ����x
	static const float s_PLAYER_TURN_SPEED; //!< �v���C���[�̌�����ς��鑬�x
	//static const float GameConfiguration::s_PLAYER_COLLIDE_CHANGE_COLOR_TIME; //!< �v���C���[���G�ƏՓˎ��ɐF���ς�鎞��
	static const D3DXVECTOR3 s_PLAYER_DEFAULT_POSITION; //!< �v���C���[�̃f�t�H���g�̈ʒu
	static const D3DXVECTOR3 s_PLAYER_DEFAULT_SCALE; //!< �v���C���[�̃f�t�H���g�̏k��
	static const D3DXVECTOR3 s_PLAYER_DEFAULT_ROTATION; //!< �v���C���[�̃f�t�H���g�̌���
	// collision
	static const float s_PLAYER_COLLISION_RADIUS; //!< �v���C���[�̏Փ˔��a
	// mesh
	//static const acut::TString s_PLAYER_MESH_FOLDER_PATH; //!< �v���C���[�̃��b�V���t�@�C�����i�[����Ă���t�H���_�p�X
	static const acut::TString s_PLAYER_MESH_FILE_PATH; //!< �v���C���[�̃��b�V���t�@�C���̃p�X
	// sound
	static const acut::TString s_PLAYER_COLLISION_SOUND_FILE_PATH; //!< �Փˎ��̌��ʉ��t�@�C���ւ̃p�X

private:
	// �C���X�^���X�����Ή�
	GameConfiguration();
	~GameConfiguration();
};



#endif // #ifndef ACUT_GAME_CONFIGURATION_H__
