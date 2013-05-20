#include "../acut.h"



namespace acut {

	/**
	 * @brief strdup()のような関数
	 * 引数に渡された文字列の長さ+1のメモリを確保し、確保した領域へコピーする。
	 * コピーした領域へのポインタを返す。
	 * この関数を使用する時は、メモリの開放処理忘れに注意する。
	 * 可能なら使用しない。
	 *
	 * @param pSourceName コピー元文字列
	 * @return コピー先文字列へのポインタ
	 */
	char* MemoryUtility::copyStringToHeap(const char* const pSourceString)
	{
		// NULLの場合、strlen()でアクセス違反エラーが起こるのでチェックする
		if (NULL == pSourceString) { 
			return NULL;
		}

	    size_t length = strlen(pSourceString) + 1;
	    char* pNewString = new char[length];
		if (NULL != pNewString) {
	        strcpy_s(pNewString, length, pSourceString);
		}

	    return pNewString;
	}

};
