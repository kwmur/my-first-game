#ifndef ACUT_TSTRING_H__
#define ACUT_TSTRING_H__



#include <string>
#include <tchar.h>


namespace acut {


	/**
	 * @brief TCHARに対応した文字列クラス
	 *
	 */
	typedef std::basic_string<TCHAR> TString;


};



#endif // #ifndef ACUT_TSTRING_H__
