#ifndef ACUT_COLLISION_H__
#define ACUT_COLLISION_H__



#include <windows.h>
struct D3DXVECTOR3;


namespace acut {


	class Collision {
	public:
		static bool calculateParticleCollision(
			FLOAT rA,
			FLOAT rB,
			D3DXVECTOR3 *pPre_pos_A,
			D3DXVECTOR3 *pPos_A,
			D3DXVECTOR3 *pPre_pos_B,
			D3DXVECTOR3 *pPos_B,
			FLOAT *pOut_t,
			D3DXVECTOR3 *pOut_colli_A,
			D3DXVECTOR3 *pOut_colli_B
		);

		//static bool detectParticle(D3DXVECTOR3 position1, float radius1, D3DXVECTOR3 position2, float radius2);

	private:
		// インスタンス化は未対応
		Collision();
		~Collision();
		Collision(const Collision&);
		Collision& operator=(const Collision&);
	};


};



#endif // #ifndef ACUT_COLLISION_H__
