#include "../acut.h"



namespace acut {

	/**
	 * @brief strdup()�̂悤�Ȋ֐�
	 * �����ɓn���ꂽ������̒���+1�̃��������m�ۂ��A�m�ۂ����̈�փR�s�[����B
	 * �R�s�[�����̈�ւ̃|�C���^��Ԃ��B
	 * ���̊֐����g�p���鎞�́A�������̊J�������Y��ɒ��ӂ���B
	 * �\�Ȃ�g�p���Ȃ��B
	 *
	 * @param pSourceName �R�s�[��������
	 * @return �R�s�[�敶����ւ̃|�C���^
	 */
	char* MemoryUtility::copyStringToHeap(const char* const pSourceString)
	{
		// NULL�̏ꍇ�Astrlen()�ŃA�N�Z�X�ᔽ�G���[���N����̂Ń`�F�b�N����
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
