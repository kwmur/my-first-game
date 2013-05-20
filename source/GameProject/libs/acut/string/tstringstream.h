#ifndef ACUT_T_STRING_STREAM_H__
#define ACUT_T_STRING_STREAM_H__



#include <sstream>


namespace acut {


	/**
	 * @brief 文字列ストリームクラスのtypedef
	 *
	 */
#ifdef UNICODE
	typedef std::wstringstream tstringstream;
#else
	typedef std::stringstream tstringstream;
#endif


};



#endif // #ifndef ACUT_T_STRING_STREAM_H__
