#include "../acut.h"
//#include "Collision.h"
//#include <d3dx9.h>


namespace acut {


	/**
	 * @brief パーティクル衝突判定・時刻・位置算出関数
	 *
	 * 参考Webサイト
	 *   Webサイト名: マルペケつくろーどっとコム
	 *   URI: http://marupeke296.com/index.html
	 *   License: http://marupeke296.com/InfomationOfOX.html
	 * 参考Webページ
	 *   Webページタイトル: その９　移動する２つの球の衝突場所と時刻を得る
	 *   URI: http://marupeke296.com/COL_3D_No9_GetSphereColliTimeAndPos.html
	 * 
	 * @param rA           : パーティクルAの半径
	 * @param rB           : パーティクルBの半径
	 * @param pre_pos_A    : パーティクルAの前の位置
	 * @param pos_A        : パーティクルAの次の到達位置
	 * @param pre_pos_B    : パーティクルBの前位置
	 * @param pos_B        : パーティクルBの次の到達位置
	 * @param pout_t       : 衝突時間を格納するFLOAT型へのポインタ
	 * @param pout_colli_A : パーティクルAの衝突位置を格納するD3DXVECTOR型へのポインタ
	 * @param pout_colli_B : パーティクルBの衝突位置を格納するD3DXVECTOR型へのポインタ
	 * @return 衝突している: true、 衝突していない: false
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
	   // 前位置及び到達位置におけるパーティクル間のベクトルを算出
	   D3DXVECTOR3 C0 = *pPre_pos_B - *pPre_pos_A;
	   D3DXVECTOR3 C1 = *pPos_B - *pPos_A;
	   D3DXVECTOR3 D = C1 - C0;

	   // 衝突判定用の2次関数係数の算出
	   FLOAT P = D3DXVec3LengthSq( &D ); if(P==0) return false; // 同じ方向に移動
	   FLOAT Q = D3DXVec3Dot( &C0, &D );
	   FLOAT R = D3DXVec3LengthSq( &C0 );

	   // パーティクル距離
	   FLOAT r = rA + rB;

	   // 衝突判定式
	   FLOAT Judge = Q*Q - P*(R-r*r);
	   if( Judge < 0 ){
	      // 衝突していない
	      return false;
	   }

	   // 衝突時間の算出
	   FLOAT t_plus = (-Q + sqrt(Judge))/P;
	   FLOAT t_minus = (-Q - sqrt(Judge))/P;

	   // 衝突時間が0未満1より大きい場合、衝突しない
	   if( (t_plus < 0 || t_plus > 1) && (t_minus < 0 || t_minus > 1)) return false;

	   // 衝突時間の決定（t_minus側が常に最初の衝突）
	   *pOut_t = t_minus;

	   // 衝突位置の決定
	   *pOut_colli_A = *pPre_pos_A + t_minus * (*pPos_A - *pPre_pos_A);
	   *pOut_colli_B = *pPre_pos_B + t_minus * (*pPos_B - *pPre_pos_B);

	   return true; // 衝突報告
	}


};
