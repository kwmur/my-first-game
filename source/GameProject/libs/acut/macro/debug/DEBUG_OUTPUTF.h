#ifndef ACUT_DEBUG_OUTPUT_F_H__
#define ACUT_DEBUG_OUTPUT_F_H__



/**
 * @brief �f�o�b�O�p������o�̓}�N��
 *
 * �Q�lWeb�T�C�g
 *   Web�T�C�g��: �}���y�P����[�ǂ��ƃR��
 *   URI: http://marupeke296.com/index.html
 *   License: http://marupeke296.com/InfomationOfOX.html
 * �Q�lWeb�y�[�W
 *   Web�y�[�W�^�C�g��: ���̂R �f�o�b�O�E�B���h�E��m��Ȃ��Ƒ�ςł�
 *   URI: http://marupeke296.com/DBG_No3_OutputDebugWindow.html
 *
 */
#if defined(DEBUG) | defined(_DEBUG)



#define DEBUG_OUTPUTF(formatString, ...) \
		{ \
			TCHAR debugString__[1024]; \
			_stprintf_s(debugString__, _T(formatString), __VA_ARGS__); \
			OutputDebugString(debugString__); \
		}



#else // #if defined(DEBUG) | defined(_DEBUG)



#define DEBUG_OUTPUTF(str, ...) // �����



#endif // #if defined(DEBUG) | defined(_DEBUG)



#endif // #ifndef ACUT_DEBUG_OUTPUT_F_H__
