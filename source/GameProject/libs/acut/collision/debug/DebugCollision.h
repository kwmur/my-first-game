#ifndef ACUT_DEBUG_COLLISION_H__
#define ACUT_DEBUG_COLLISION_H__



#if defined(DEBUG) | defined(_DEBUG)



#include <d3dx9.h>

class Collision;


namespace acut {


	class DebugCollision {
	public:
		DebugCollision();
		~DebugCollision();
		void finalize();
		void render();

		void changeMeshSphere(const FLOAT radius);

	private:
		// �R�s�[�͖��Ή�
		DebugCollision(const Collision&);
		DebugCollision& operator=(const Collision&);

		void release();

		LPD3DXMESH m_pMesh; //!< ���b�V��
		D3DMATERIAL9 m_materials; //!< �}�e���A��

		// static �����o
		static const UINT s_SLICES;
		static const UINT s_STACKS;
	};


};



#endif // #if defined(DEBUG) | defined(_DEBUG)



#endif // #ifndef ACUT_DEBUG_COLLISION_H__
