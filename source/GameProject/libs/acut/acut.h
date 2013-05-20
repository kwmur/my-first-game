/** @file
 * @brief
 * acut (Alarm Clock Utilities) version 0.001
 * acut�̂��ׂẴw�b�_�t�@�C����include�����w�b�_�t�@�C���B
 *
 */

#pragma once


// acut�Ŏg�p���Ă���W���w�b�_�t�@�C��
// C++
#include <vector>
#include <string>
#include <algorithm>
// Windows
//#define NOMINMAX // std::min() std::max()���g�p�o����悤�ɂ���
#include <windows.h>
#include <tchar.h>


// acut�Ŏg�p���Ă���O�����C�u�������̃w�b�_�t�@�C��
// DirectX
#include <d3d9.h>
#include <d3dx9.h>
// DXUT
#include "DXUT.h"
#include "SDKmisc.h"
#include "SDKsound.h"
// Boost


// acut�̃w�b�_�t�@�C��
// string
#include "string/TString.h"
#include "string/tstringstream.h"
// functor
#include "functor/Release.h"
// exception
#include "exception/Exception.h"
#include "exception/ExceptionHandler.h"
// debug
#include "macro/debug/DEBUG_OUTPUTF.h"
// memory
#include "memory/MemoryUtility.h"
// mesh
#include "mesh/MeshUtility.h"
// collision
#include "collision/Collision.h"
#include "collision/debug/DebugCollision.h"
// macro
#include "macro/V_THROW.h"
// sound
#include "sound/JukeBox.h"
#include "sound/SoundUtility.h"
// animation
#include "animation/AnimationMeshContainer.h"
#include "animation/AllocateHierarchy.h"
#include "animation/AnimationFrame.h"
#include "animation/AnimationManager.h"
// actor
//#include "actor/Player.h"

//physics
#include "physics/BulletPhysics/BulletPhysics.h"


