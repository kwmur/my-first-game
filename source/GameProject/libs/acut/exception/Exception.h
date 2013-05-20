#ifndef ACUT_EXCEPTION_H__
#define ACUT_EXCEPTION_H__



#include "../string/TString.h"


namespace acut {

	/**
	 * @brief 例外クラス
	 * TODO: boost::exception 使用する
	 *
	 */
	class Exception {    
	public:
		Exception();
		Exception(const acut::TString file, const int line, const acut::TString function);
		~Exception();

		acut::TString what() const;

	private:
		// コピーは未定義
		//Exception(const acut::Exception& rhs);
		//Exception& operator=(const Exception& rhs);

		const acut::TString m_file;
		const int m_line;
		const acut::TString m_function;
	};

};



#endif // #ifndef ACUT_EXCEPTION_H__
