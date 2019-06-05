#define UNICODE

#include "main.h"
#include "string.h"
#include "stdio.h"
#include "oleauto.h"
#include "SerialC/SerialC.h"

HANDLE hComm = INVALID_HANDLE_VALUE;

void DLL_EXPORT __stdcall WriteStrToComm(LPCSTR toWrite){
    SerialC_Write(hComm, (uint8_t*)toWrite, strlen(toWrite));
}



BSTR DLL_EXPORT __stdcall ReadStrFromComm(){
    OLECHAR* arr;
    DWORD size;
    SerialC_Read(hComm, (uint8_t**)&arr, &size);
    BSTR ret = SysAllocString(arr);
    free(arr);
    return ret;
}


void DLL_EXPORT __stdcall Init(const INT32 ComID){
    SerialC_Init(&hComm, (uint16_t)ComID, 9600, 8, 0, 0);
}





DLL_EXPORT BOOL APIENTRY __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch (fdwReason)
    {
        case DLL_PROCESS_ATTACH:
            // attach to process
            // return FALSE to fail DLL load
            break;

        case DLL_PROCESS_DETACH:
            SerialC_Free(hComm);
            break;

        case DLL_THREAD_ATTACH:
            // attach to thread
            break;

        case DLL_THREAD_DETACH:
            // detach from thread
            break;
    }
    return TRUE; // succesful
}
