#ifndef ACUT_V_THROW_H__
#define ACUT_V_THROW_H__



#if defined(DEBUG) | defined(_DEBUG)


#ifndef V_THROW
#define V_THROW(x) \
		{ \
			HRESULT hr = (x); \
			if (FAILED(hr)) { \
				DXUTTrace(__FILE__, static_cast<DWORD>(__LINE__), hr, L#x, true); \
				throw acut::Exception(__FILEW__, __LINE__, __FUNCTIONW__); \
			} \
		}
#endif // #ifndef V_THROW

#else // #if defined(DEBUG) | defined(_DEBUG)
#ifndef V_THROW
#define V_THROW(x) \
		{ \
			if (FAILED((x))) { \
				throw acut::Exception(__FILEW__, __LINE__, __FUNCTIONW__); \
			} \
		}
#endif // #ifndef V_THROW


#endif // #if defined(DEBUG) | defined(_DEBUG)



#endif // #ifndef ACUT_V_THROW_H__
