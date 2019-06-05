#ifndef __MAIN_H__
#define __MAIN_H__

#include <windows.h>

/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C"
{
#endif

SAFEARRAY* DLL_EXPORT __stdcall ReadRawFromComm();

void DLL_EXPORT __stdcall Init(const INT32 ComID);

void DLL_EXPORT __stdcall WriteComm(const LPCSTR toWrite);

BSTR DLL_EXPORT __stdcall ReadComm();

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H__
