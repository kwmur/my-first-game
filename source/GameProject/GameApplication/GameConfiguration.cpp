#include "GameConfiguration.h"



// Game
const acut::TString GameConfiguration::s_GAME_TITLE = _T("Star Attack - [Esc]:終了 [W][S][A][D]:移動 左クリック:ショット マウス:視点移動");
//const int GameConfiguration::s_BACK_BUFFER_WIDTH = 1900;
const int GameConfiguration::s_BACK_BUFFER_WIDTH = 800;
//const int GameConfiguration::s_BACK_BUFFER_HEIGHT = 1200;
const int GameConfiguration::s_BACK_BUFFER_HEIGHT = 600;

// TitleScene
const acut::TString GameConfiguration::s_TITLE_SCENE_BGM_FILE_PATH = _T("media\\sound\\bgm_maoudamashii_neorock05.wav");
const acut::TString GameConfiguration::s_TITLE_SCENE_IMAGE_FILE_PATH = _T("media\\images\\title_800x600.bmp");

// MainScene
const acut::TString GameConfiguration::s_MAIN_SCENE_BGM_FILE_PATH = _T("media\\sound\\bgm_maoudamashii_cyber01.wav");

// ResultScene
const acut::TString GameConfiguration::s_RESULT_SCENE_BGM_FILE_PATH = _T("media\\sound\\bgm_maoudamashii_cyber02.wav");
const acut::TString GameConfiguration::s_RESULT_SCENE_IMAGE_FILE_PATH = _T("media\\images\\result_800x600.bmp");

// Floor
const float GameConfiguration::s_FLOOR_MOVE_RADIUS = 8.6f;
// mesh
const acut::TString GameConfiguration::s_FLOOR_MESH_FOLDER_PATH = _T("media\\meshes\\floor\\");
const acut::TString GameConfiguration::s_FLOOR_MESH_FILE_PATH = _T("media\\meshes\\floor\\floor.x");

// Bullet
const float GameConfiguration::s_BULLET_SPEED = 4.5f;
const float GameConfiguration::s_BULLET_DISAPPEARANCE_TIME = 1.5f;
const double GameConfiguration::s_BULLET_FIRING_INTERVAL = 0.12;
const float GameConfiguration::s_BULLET_OFFSET_POSITION_Y = 0.18f;
const size_t GameConfiguration::s_BULLET_MAX = 50;
// collision
const float GameConfiguration::s_BULLET_COLLISION_RADIUS = 0.06f;
// mesh
const acut::TString GameConfiguration::s_BULLET_MESH_FOLDER_PATH = _T("media\\meshes\\bullet\\");
const acut::TString GameConfiguration::s_BULLET_MESH_FILE_PATH = _T("media\\meshes\\bullet\\bullet.x");
// sound
const acut::TString GameConfiguration::s_BULLET_SHOT_SOUND_FILE_PATH = _T("media\\sound\\se_maoudamashii_battle_gun01.wav");


// Enemy
const float GameConfiguration::s_ENEMY_SPEED = 1.3f;
const int GameConfiguration::s_ENEMY_HEALTH = 1;
const float GameConfiguration::s_ENEMY_DIE_OUT_TIME = 1.0f;
const float GameConfiguration::s_ENEMY_GENERATE_INTERVAL = 0.2f;
const size_t GameConfiguration::s_ENEMY_MAX = 40;
const float GameConfiguration::s_ENEMY_GENERATE_DISTANCE = 5.0f;
const float GameConfiguration::s_ENEMY_GENERATE_POSITION_Y_1 = 0.5f;
const float GameConfiguration::s_ENEMY_GENERATE_POSITION_Y_2 = 5.5f;
const int GameConfiguration::s_ENEMY_GENERATE_RANDOM_DIVISION = 15;
const int GameConfiguration::s_ENEMY_GENERATE_RANDOM_SUBTRACTION = 5;
// collision
const float GameConfiguration::s_ENEMY_COLLISION_RADIUS = 0.4f;
// mesh
//const acut::TString GameConfiguration::s_ENEMY_MESH_FOLDER_PATH = _T("media\\meshes\\tiger\\");
//const acut::TString GameConfiguration::s_ENEMY_MESH_FILE_PATH = _T("media\\meshes\\tiger\\tiger.x");
const acut::TString GameConfiguration::s_ENEMY_MESH_FOLDER_PATH = _T("media\\meshes\\enemy\\");
const acut::TString GameConfiguration::s_ENEMY_MESH_FILE_PATH = _T("media\\meshes\\enemy\\enemy.x");
// sound
const acut::TString GameConfiguration::s_ENEMY_COLLISION_SOUND_FILE_PATH = _T("media\\sound\\se_maoudamashii_battle03.wav");


// Player
const int GameConfiguration::s_PLAYER_HEALTH = 3;
const float GameConfiguration::s_PLAYER_SPEED = 1.0f;
const float GameConfiguration::s_PLAYER_TURN_SPEED = 0.5f;
//const float GameConfiguration::s_PLAYER_COLLIDE_CHANGE_COLOR_TIME = 1.0f;
const D3DXVECTOR3 GameConfiguration::s_PLAYER_DEFAULT_POSITION(0.5f, 0.0035f, 0.5f);
const D3DXVECTOR3 GameConfiguration::s_PLAYER_DEFAULT_SCALE(0.0004f, 0.0004f, 0.0004f);
//const D3DXVECTOR3 GameConfiguration::s_PLAYER_DEFAULT_ROTATION(-D3DX_PI / 2.0f, 0.0f, 0.0f); // tyny_4anim.xの場合
const D3DXVECTOR3 GameConfiguration::s_PLAYER_DEFAULT_ROTATION(0.0f, -D3DX_PI, 0.0f);
// collision
const float GameConfiguration::s_PLAYER_COLLISION_RADIUS = 0.02f;
// mesh
//const acut::TString GameConfiguration::s_PLAYER_MESH_FILE_PATH = _T("media\\meshes\\Tiny\\tiny_4anim.x"); // sample
const acut::TString GameConfiguration::s_PLAYER_MESH_FILE_PATH = _T("media\\meshes\\player\\player.x");
// sound
const acut::TString GameConfiguration::s_PLAYER_COLLISION_SOUND_FILE_PATH = _T("media\\sound\\se_maoudamashii_retro22.wav");
