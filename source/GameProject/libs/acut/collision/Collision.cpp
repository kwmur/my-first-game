#include "../acut.h"
//#include "Collision.h"
//#include <d3dx9.h>


namespace acut {


	/**
	 * @brief �p�[�e�B�N���Փ˔���E�����E�ʒu�Z�o�֐�
	 *
	 * �Q�lWeb�T�C�g
	 *   Web�T�C�g��: �}���y�P����[�ǂ��ƃR��
	 *   URI: http://marupeke296.com/index.html
	 *   License: http://marupeke296.com/InfomationOfOX.html
	 * �Q�lWeb�y�[�W
	 *   Web�y�[�W�^�C�g��: ���̂X�@�ړ�����Q�̋��̏Փˏꏊ�Ǝ����𓾂�
	 *   URI: http://marupeke296.com/COL_3D_No9_GetSphereColliTimeAndPos.html
	 * 
	 * @param rA           : �p�[�e�B�N��A�̔��a
	 * @param rB           : �p�[�e�B�N��B�̔��a
	 * @param pre_pos_A    : �p�[�e�B�N��A�̑O�̈ʒu
	 * @param pos_A        : �p�[�e�B�N��A�̎��̓��B�ʒu
	 * @param pre_pos_B    : �p�[�e�B�N��B�̑O�ʒu
	 * @param pos_B        : �p�[�e�B�N��B�̎��̓��B�ʒu
	 * @param pout_t       : �Փˎ��Ԃ��i�[����FLOAT�^�ւ̃|�C���^
	 * @param pout_colli_A : �p�[�e�B�N��A�̏Փˈʒu���i�[����D3DXVECTOR�^�ւ̃|�C���^
	 * @param pout_colli_B : �p�[�e�B�N��B�̏Փˈʒu���i�[����D3DXVECTOR�^�ւ̃|�C���^
	 * @return �Փ˂��Ă���: true�A �Փ˂��Ă��Ȃ�: false
	 *
	 */
	bool Collision::calculateParticleCollision(
	   FLOAT rA,
	   FLOAT rB,
	   D3DXVECTOR3 *pPre_pos_A,
	   D3DXVECTOR3 *pPos_A,
	   D3DXVECTOR3 *pPre_pos_B,
	   D3DXVECTOR3 *pPos_B,
	   FLOAT *pOut_t,
	   D3DXVECTOR3 *pOut_colli_A,
	   D3DXVECTOR3 *pOut_colli_B
	)
	{
	   // �O�ʒu�y�ѓ��B�ʒu�ɂ�����p�[�e�B�N���Ԃ̃x�N�g�����Z�o
	   D3DXVECTOR3 C0 = *pPre_pos_B - *pPre_pos_A;
	   D3DXVECTOR3 C1 = *pPos_B - *pPos_A;
	   D3DXVECTOR3 D = C1 - C0;

	   // �Փ˔���p��2���֐��W���̎Z�o
	   FLOAT P = D3DXVec3LengthSq( &D ); if(P==0) return false; // ���������Ɉړ�
	   FLOAT Q = D3DXVec3Dot( &C0, &D );
	   FLOAT R = D3DXVec3LengthSq( &C0 );

	   // �p�[�e�B�N������
	   FLOAT r = rA + rB;

	   // �Փ˔��莮
	   FLOAT Judge = Q*Q - P*(R-r*r);
	   if( Judge < 0 ){
	      // �Փ˂��Ă��Ȃ�
	      return false;
	   }

	   // �Փˎ��Ԃ̎Z�o
	   FLOAT t_plus = (-Q + sqrt(Judge))/P;
	   FLOAT t_minus = (-Q - sqrt(Judge))/P;

	   // �Փˎ��Ԃ�0����1���傫���ꍇ�A�Փ˂��Ȃ�
	   if( (t_plus < 0 || t_plus > 1) && (t_minus < 0 || t_minus > 1)) return false;

	   // �Փˎ��Ԃ̌���it_minus������ɍŏ��̏Փˁj
	   *pOut_t = t_minus;

	   // �Փˈʒu�̌���
	   *pOut_colli_A = *pPre_pos_A + t_minus * (*pPos_A - *pPre_pos_A);
	   *pOut_colli_B = *pPre_pos_B + t_minus * (*pPos_B - *pPre_pos_B);

	   return true; // �Փ˕�
	}


};
