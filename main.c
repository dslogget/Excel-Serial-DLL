#define UNICODE

#include "main.h"
#include "string.h"
#include "stdio.h"
#include "oleauto.h"

HANDLE hComm = INVALID_HANDLE_VALUE;

void DLL_EXPORT __stdcall WriteComm(const LPCSTR toWrite){
//    char buffer [1000] = {0};
    DWORD bytesWritten = 0;

    //WideCharToMultiByte(CP_UTF8, 0, (LPCWSTR)(toWrite), -1, buffer, 1000, NULL, &unkown);
    //MessageBoxA(0, toWrite, "DLL Message", MB_OK | MB_ICONINFORMATION);


    WriteFile(hComm, toWrite, strlen(toWrite), &bytesWritten, NULL);
}



BSTR DLL_EXPORT __stdcall ReadComm(){
    OLECHAR buffer [1000] = {0};
    DWORD bytesRead = 0;

    ReadFile(hComm, buffer, 999, &bytesRead, NULL);


    return SysAllocString(buffer);
}


void DLL_EXPORT __stdcall Init(const INT32 ComID){
    if(hComm == INVALID_HANDLE_VALUE){
        char buf[100];
        sprintf(buf, "\\\\.\\COM%d", ComID);
        hComm = CreateFileA(buf, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

        if (hComm == INVALID_HANDLE_VALUE){
            MessageBoxA(0, "Port Failed to Open", "DLL Message", MB_OK | MB_ICONINFORMATION);
        }else{
            //MessageBoxA(0, "Port Opened", "DLL Message", MB_OK | MB_ICONINFORMATION);
        }

        DCB dcbSerialParams = { 0 }; // Initializing DCB structure
        dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
        GetCommState(hComm, &dcbSerialParams);
        dcbSerialParams.BaudRate = CBR_9600;  // Setting BaudRate = 9600
        dcbSerialParams.ByteSize = 8;         // Setting ByteSize = 8
        dcbSerialParams.StopBits = ONESTOPBIT;// Setting StopBits = 1
        dcbSerialParams.Parity   = NOPARITY;  // Setting Parity = None
        SetCommState(hComm, &dcbSerialParams);

        COMMTIMEOUTS timeouts = { 0 };
        timeouts.ReadIntervalTimeout         = 50; // in milliseconds
        timeouts.ReadTotalTimeoutConstant    = 50; // in milliseconds
        timeouts.ReadTotalTimeoutMultiplier  = 10; // in milliseconds
        timeouts.WriteTotalTimeoutConstant   = 50; // in milliseconds
        timeouts.WriteTotalTimeoutMultiplier = 10; // in milliseconds

        SetCommTimeouts(hComm, &timeouts);
    }


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
            CloseHandle(hComm);
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
