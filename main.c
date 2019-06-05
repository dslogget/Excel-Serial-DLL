#define UNICODE

#include "main.h"
#include "string.h"
#include "stdio.h"
#include "oleauto.h"
#include "SerialC/SerialC.h"

HANDLE hComm = INVALID_HANDLE_VALUE;


SAFEARRAY* DLL_EXPORT __stdcall ReadRawFromComm(){
    SAFEARRAYBOUND bounds = {0};
    bounds.lLbound = 1;
    uint8_t* arr;
    uint8_t* loc;
    DWORD size;
    SerialC_Read(hComm, &arr, &size);
    bounds.cElements = size;
    //char buf[100];
    //sprintf(buf, "%lu elements", bounds.cElements);
    //MessageBoxA(0, buf, "DLL Message", MB_OK | MB_ICONINFORMATION);
    SAFEARRAY* ret = SafeArrayCreate(VT_UI1, 1, &bounds);
    SafeArrayAccessData(ret, (void**)&loc);
    memcpy(loc, arr, bounds.cElements);
    free(arr);
    return ret;
}


void DLL_EXPORT __stdcall WriteRawToComm(SAFEARRAY** arr){
    VARTYPE type;
    SafeArrayGetVartype(*arr, &type);
    if(type == VT_UI1){
        uint8_t *data;
        long uBound, lBound;
        SafeArrayAccessData(*arr, (void**)&data);
        SafeArrayGetUBound(*arr, 1, &uBound);
        SafeArrayGetLBound(*arr, 1, &lBound);


        SerialC_Write(hComm, data, (DWORD)(1 + uBound - lBound));
    }else{
        MessageBoxA(0, "Invalid type", "DLL Message", MB_OK | MB_ICONINFORMATION);
    }
}

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
