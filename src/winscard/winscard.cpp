// cl.exe /Ox /GL /arch:SSE2 /LD /MD WinSCard.cpp (ランタイム別腹)
// cl.exe /Ox /GL /arch:SSE2 /LD /MT WinSCard.cpp (ランタイム内蔵)
//  winscard.cpp Put the following files in the same folder:
//      Crypto.cpp, Crypto.h, Decoder-ECM.cpp, Decoder.h, Global.h
//      Keys.cpp, Keys.h, Keyset.cpp, Keyset.h
//  You should modify "Size" in Decoder-ECM.cpp l42 to "Size-3"

#include <Windows.h>
#include <stdlib.h>
#include <time.h>
#define htons htons_
#define ntohs ntohs_
#include "Crypto.cpp"
#include "Decoder-ECM.cpp"
#include "Keys.cpp"
#include "Keyset.cpp"
#ifdef _WIN64
#pragma comment(linker,"/EXPORT:SCardEstablishContext=SCardEstablishContext_")
#pragma comment(linker,"/EXPORT:SCardListReadersA=SCardListReadersA_")
#pragma comment(linker,"/EXPORT:SCardListReadersW=SCardListReadersW_")
#pragma comment(linker,"/EXPORT:SCardConnectA=SCardConnectA_")
#pragma comment(linker,"/EXPORT:SCardConnectW=SCardConnectW_")
#pragma comment(linker,"/EXPORT:SCardTransmit=SCardTransmit_")
#pragma comment(linker,"/EXPORT:SCardDisconnect=SCardDisconnect_")
#pragma comment(linker,"/EXPORT:SCardFreeMemory=SCardFreeMemory_")
#pragma comment(linker,"/EXPORT:SCardGetStatusChangeA=SCardGetStatusChangeA_")
#pragma comment(linker,"/EXPORT:SCardGetStatusChangeW=SCardGetStatusChangeW_")
#pragma comment(linker,"/EXPORT:SCardReleaseContext=SCardReleaseContext_")
#pragma comment(linker,"/EXPORT:SCardReconnect=SCardReconnect_")
#pragma comment(linker,"/EXPORT:SCardAccessStartedEvent=SCardAccessStartedEvent_")
#pragma comment(linker,"/EXPORT:SCardReleaseStartedEvent=SCardReleaseStartedEvent_")
#pragma comment(linker,"/EXPORT:SCardCancel=SCardCancel_")
#pragma comment(linker,"/EXPORT:g_rgSCardT1Pci=g_rgSCardT1Pci_")
#else
#pragma comment(linker,"/EXPORT:SCardEstablishContext=_SCardEstablishContext_@16")
#pragma comment(linker,"/EXPORT:SCardListReadersA=_SCardListReadersA_@16")
#pragma comment(linker,"/EXPORT:SCardListReadersW=_SCardListReadersW_@16")
#pragma comment(linker,"/EXPORT:SCardConnectA=_SCardConnectA_@24")
#pragma comment(linker,"/EXPORT:SCardConnectW=_SCardConnectW_@24")
#pragma comment(linker,"/EXPORT:SCardTransmit=_SCardTransmit_@28")
#pragma comment(linker,"/EXPORT:SCardDisconnect=_SCardDisconnect_@8")
#pragma comment(linker,"/EXPORT:SCardFreeMemory=_SCardFreeMemory_@8")
#pragma comment(linker,"/EXPORT:SCardGetStatusChangeA=_SCardGetStatusChangeA_@16")
#pragma comment(linker,"/EXPORT:SCardGetStatusChangeW=_SCardGetStatusChangeW_@16")
#pragma comment(linker,"/EXPORT:SCardReleaseContext=_SCardReleaseContext_@4")
#pragma comment(linker,"/EXPORT:SCardReconnect=_SCardReconnect_@20")
#pragma comment(linker,"/EXPORT:SCardAccessStartedEvent=_SCardAccessStartedEvent_@0")
#pragma comment(linker,"/EXPORT:SCardReleaseStartedEvent=_SCardReleaseStartedEvent_@0")
#pragma comment(linker,"/EXPORT:SCardCancel=_SCardCancel_@4")
#pragma comment(linker,"/EXPORT:g_rgSCardT1Pci=_g_rgSCardT1Pci_")
#endif
static HANDLE g_SCardStartedEvent_ = NULL;
static BYTE pbIDBuffer[]={0x4d,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
BOOL WINAPI DllMain(HINSTANCE hinstDLL,DWORD fdwReason,LPVOID lpvReserved)
{
    u16 id01,id23,id45,id67;
    srand(time(NULL) & 0xffff);
    srand(rand());
    u16 MorT = rand() % 2;
    if (!MorT) pbIDBuffer[0]=0x54;
    id01=0x0007;
    id23=rand();
    id45=rand();
    id67=id01 ^ id23 ^ id45;
    pbIDBuffer[2]=id01>>8;
    pbIDBuffer[3]=id01&0xff;
    pbIDBuffer[4]=id23>>8;
    pbIDBuffer[5]=id23&0xff;
    pbIDBuffer[6]=id45>>8;
    pbIDBuffer[7]=id45&0xff;
    pbIDBuffer[8]=id67>>8;
    pbIDBuffer[9]=id67&0xff;

    if (fdwReason==DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hinstDLL);
        g_SCardStartedEvent_ = CreateEvent(NULL, TRUE, TRUE, NULL);
        BCAS::Keys::RegisterAll();
    }
    else if (fdwReason==DLL_PROCESS_DETACH)
    {
        if (g_SCardStartedEvent_) CloseHandle(g_SCardStartedEvent_);
    }
    return TRUE;
}
extern "C"{
LONG WINAPI SCardEstablishContext_(DWORD,LPCVOID,LPCVOID,LPSCARDCONTEXT phContext)
{
    *phContext=(SCARDCONTEXT)0x35313239;
    return SCARD_S_SUCCESS;
}
LONG WINAPI SCardListReadersA_(SCARDCONTEXT,LPCTSTR,LPSTR mszReaders,LPDWORD pcchReaders)
{
    static CHAR r[]="Gemplus USB Smart Card Reader 0\0";
    if(mszReaders)if(*pcchReaders==SCARD_AUTOALLOCATE)*(LPSTR*)mszReaders=r;
    else memcpy(mszReaders,r,sizeof r);
    *pcchReaders=sizeof r/sizeof r[0];
    return SCARD_S_SUCCESS;
}
LONG WINAPI SCardListReadersW_(SCARDCONTEXT,LPCTSTR,LPWSTR mszReaders,LPDWORD pcchReaders)
{
    static WCHAR r[]=L"Gemplus USB Smart Card Reader 0\0";
    if(mszReaders)if(*pcchReaders==SCARD_AUTOALLOCATE)*(LPWSTR*)mszReaders=r;
    else memcpy(mszReaders,r,sizeof r);
    *pcchReaders=sizeof r/sizeof r[0];
    return SCARD_S_SUCCESS;
}
LONG WINAPI SCardConnectA_(SCARDCONTEXT,LPCTSTR,DWORD,DWORD,LPSCARDHANDLE phCard,LPDWORD pdwActiveProtocol)
{
    *phCard=(SCARDHANDLE)0x35313239;
    *pdwActiveProtocol=SCARD_PROTOCOL_T1;
    return SCARD_S_SUCCESS;
}
LONG WINAPI SCardConnectW_(SCARDCONTEXT,LPCTSTR,DWORD,DWORD,LPSCARDHANDLE phCard,LPDWORD pdwActiveProtocol)
{
    *phCard=(SCARDHANDLE)0x35313239;
    *pdwActiveProtocol=SCARD_PROTOCOL_T1;
    return SCARD_S_SUCCESS;
}
LONG WINAPI SCardTransmit_(SCARDHANDLE,LPCSCARD_IO_REQUEST,LPCBYTE pbSendBuffer,DWORD cbSendLength,LPSCARD_IO_REQUEST,LPBYTE pbRecvBuffer,LPDWORD pcbRecvLength)
{
    switch(pbSendBuffer[1])
    {
    case 0x30:{ // 90 30 00 00 00
        BYTE r[]={ // INT 初期設定条件
            0x00,0x39,0x00,0x00,0x21,0x00,0x00,0x05, 0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x50,
            0x36,0x31,0x04,0x66,0x4b,0x17,0xea,0x5c, 0x32,0xdf,0x9c,0xf5,0xc4,0xc3,0x6c,0x1b,
            0xec,0x99,0x39,0x21,0x68,0x9d,0x4b,0xb7, 0xb7,0x4e,0x40,0x84,0x0d,0x2e,0x7d,0x98,
            0xfe,0x27,0x19,0x99,0x19,0x69,0x09,0x11, 0x01,0x02,0x01,0x90,0x00};
        memcpy(pbRecvBuffer,r,sizeof r);
        memcpy(pbRecvBuffer+8,pbIDBuffer+2,6);
        *pcbRecvLength=sizeof r;
        break;}
    case 0x32:{ // 90 32 00 00 00
        BYTE r[]={ // IDI カードID 情報取得
            0x00,0x0f,0x00,0x00,0x21,0x00,0x01,0x4d, 0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x90,0x00};
        memcpy(pbRecvBuffer,r,sizeof r);
        memcpy(pbRecvBuffer+7,pbIDBuffer,10);
        *pcbRecvLength=sizeof r;
        break;}
    case 0x34:{ // ECM 受信
        BYTE r[]={
            0x00,0x15,0x00,0x00,0x08,0x00,0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x90, 0x00};
        BCAS::Decoder::DecodeECM(pbSendBuffer+5,pbSendBuffer[4],r+6,NULL);
        memcpy(pbRecvBuffer,r,sizeof r);
        *pcbRecvLength=sizeof r;
        break;}
    case 0x3C:{
        BYTE r[]={ // CHK 契約確認
            0x00,0x10,0x00,0x00,0x08,0x00,0x00,0x01, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x90,0x00};
        memcpy(pbRecvBuffer,r,sizeof r);
        *pcbRecvLength=sizeof r;
        break;}
    case 0x80:{
        BYTE r[]={ // WUI 省電力通電制御情報要求
            0x00,0x10,0x00,0x00,0xA1,0x01,0x00,0x00, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x90,0x00};
        memcpy(pbRecvBuffer,r,sizeof r);
        *pcbRecvLength=sizeof r;
        break;}
    default:{
        BYTE r[]={
            0x00,0x04,0x00,0x00,0xa1,0xfe,0x90,0x00};
        memcpy(pbRecvBuffer,r,sizeof r);
        *pcbRecvLength=sizeof r;
        break;}
    }
    return SCARD_S_SUCCESS;
}
LONG WINAPI SCardDisconnect_(SCARDHANDLE,DWORD){return SCARD_S_SUCCESS;}
LONG WINAPI SCardFreeMemory_(SCARDCONTEXT,LPCVOID){return SCARD_S_SUCCESS;}
LONG WINAPI SCardGetStatusChangeA_(SCARDCONTEXT,DWORD,LPSCARD_READERSTATEA rgReaderStates,DWORD){rgReaderStates->dwEventState=SCARD_STATE_PRESENT;return SCARD_S_SUCCESS;}
LONG WINAPI SCardGetStatusChangeW_(SCARDCONTEXT,DWORD,LPSCARD_READERSTATEA rgReaderStates,DWORD){rgReaderStates->dwEventState=SCARD_STATE_PRESENT;return SCARD_S_SUCCESS;}
LONG WINAPI SCardReleaseContext_(SCARDCONTEXT){return SCARD_S_SUCCESS;}
LONG WINAPI SCardReconnect_(SCARDHANDLE,DWORD,DWORD,DWORD,LPDWORD pdwActiveProtocol)
{
    *pdwActiveProtocol=SCARD_PROTOCOL_T1;
    return SCARD_S_SUCCESS;
}
HANDLE WINAPI SCardAccessStartedEvent_(void){return g_SCardStartedEvent_;}
void WINAPI SCardReleaseStartedEvent_(void) {}
LONG WINAPI SCardCancel_(SCARDCONTEXT)
{
    return SCARD_S_SUCCESS;
}
SCARD_IO_REQUEST g_rgSCardT1Pci_;
}
