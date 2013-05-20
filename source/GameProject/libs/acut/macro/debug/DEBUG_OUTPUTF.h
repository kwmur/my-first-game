#ifndef ACUT_DEBUG_OUTPUT_F_H__
#define ACUT_DEBUG_OUTPUT_F_H__



/**
 * @brief デバッグ用文字列出力マクロ
 *
 * 参考Webサイト
 *   Webサイト名: マルペケつくろーどっとコム
 *   URI: http://marupeke296.com/index.html
 *   License: http://marupeke296.com/InfomationOfOX.html
 * 参考Webページ
 *   Webページタイトル: その３ デバッグウィンドウを知らないと大変です
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



#define DEBUG_OUTPUTF(str, ...) // 空実装



#endif // #if defined(DEBUG) | defined(_DEBUG)



#endif // #ifndef ACUT_DEBUG_OUTPUT_F_H__
