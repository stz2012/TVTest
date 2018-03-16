// FSUSB2N winscard.dll wrapper (2012-09-25)->Bugfix(2012-09-27)
// Rename wrapped file to "winscard.dl_" and place in same folder.
// Refer to ARIB STD-B25 and MSDN.
#include <Windows.h>

static const TCHAR WRAPPED_NAME[] = TEXT("winscard.dl_\0##################");
static const char READER_NAME_A[] = "UnknownReader\0\0################";
static const WCHAR READER_NAME_W[] = L"UnknownReader\0\0################";

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD ul_reason_for_call, LPVOID lpReserved);

#ifdef NO_CRT
#undef RtlMoveMemory
#undef RtlCopyMemory
EXTERN_C NTSYSAPI VOID NTAPI RtlMoveMemory(VOID UNALIGNED *Dest, CONST VOID UNALIGNED *Src, SIZE_T Length);
#define RtlCopyMemory RtlMoveMemory
BOOL WINAPI DllMainCRTStartup(HINSTANCE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    return DllMain(hModule, ul_reason_for_call, lpReserved);
}
#endif

#ifdef _WIN64
#pragma comment(linker, "/EXPORT:SCardConnectA=SCardConnectA_")
#pragma comment(linker, "/EXPORT:SCardConnectW=SCardConnectW_")
#pragma comment(linker, "/EXPORT:SCardDisconnect=SCardDisconnect_")
#pragma comment(linker, "/EXPORT:SCardEstablishContext=SCardEstablishContext_")
#pragma comment(linker, "/EXPORT:SCardFreeMemory=SCardFreeMemory_")
#pragma comment(linker, "/EXPORT:SCardGetStatusChangeA=SCardGetStatusChangeA_")
#pragma comment(linker, "/EXPORT:SCardGetStatusChangeW=SCardGetStatusChangeW_")
#pragma comment(linker, "/EXPORT:SCardIsValidContext=SCardIsValidContext_")
#pragma comment(linker, "/EXPORT:SCardListReadersA=SCardListReadersA_")
#pragma comment(linker, "/EXPORT:SCardListReadersW=SCardListReadersW_")
#pragma comment(linker, "/EXPORT:SCardReleaseContext=SCardReleaseContext_")
#pragma comment(linker, "/EXPORT:SCardStatusA=SCardStatusA_")
#pragma comment(linker, "/EXPORT:SCardStatusW=SCardStatusW_")
#pragma comment(linker, "/EXPORT:SCardTransmit=SCardTransmit_")
#pragma comment(linker, "/EXPORT:g_rgSCardT1Pci=g_rgSCardT1Pci_")
#else
#pragma comment(linker, "/EXPORT:SCardConnectA=_SCardConnectA_@24")
#pragma comment(linker, "/EXPORT:SCardConnectW=_SCardConnectW_@24")
#pragma comment(linker, "/EXPORT:SCardDisconnect=_SCardDisconnect_@8")
#pragma comment(linker, "/EXPORT:SCardEstablishContext=_SCardEstablishContext_@16")
#pragma comment(linker, "/EXPORT:SCardFreeMemory=_SCardFreeMemory_@8")
#pragma comment(linker, "/EXPORT:SCardGetStatusChangeA=_SCardGetStatusChangeA_@16")
#pragma comment(linker, "/EXPORT:SCardGetStatusChangeW=_SCardGetStatusChangeW_@16")
#pragma comment(linker, "/EXPORT:SCardIsValidContext=_SCardIsValidContext_@4")
#pragma comment(linker, "/EXPORT:SCardListReadersA=_SCardListReadersA_@16")
#pragma comment(linker, "/EXPORT:SCardListReadersW=_SCardListReadersW_@16")
#pragma comment(linker, "/EXPORT:SCardReleaseContext=_SCardReleaseContext_@4")
#pragma comment(linker, "/EXPORT:SCardStatusA=_SCardStatusA_@28")
#pragma comment(linker, "/EXPORT:SCardStatusW=_SCardStatusW_@28")
#pragma comment(linker, "/EXPORT:SCardTransmit=_SCardTransmit_@28")
#pragma comment(linker, "/EXPORT:g_rgSCardT1Pci=_g_rgSCardT1Pci_")
#endif

typedef LONG (WINAPI *PFNSCardConnectA)(SCARDCONTEXT hContext,LPCSTR szReader,DWORD dwShareMode,DWORD dwPreferredProtocols,LPSCARDHANDLE phCard,LPDWORD pdwActiveProtocol);
typedef LONG (WINAPI *PFNSCardConnectW)(SCARDCONTEXT hContext,LPCWSTR szReader,DWORD dwShareMode,DWORD dwPreferredProtocols,LPSCARDHANDLE phCard,LPDWORD pdwActiveProtocol);
typedef LONG (WINAPI *PFNSCardDisconnect)(SCARDHANDLE hCard,DWORD dwDisposition);
typedef LONG (WINAPI *PFNSCardEstablishContext)(DWORD dwScope,LPCVOID pvReserved1,LPCVOID pvReserved2,LPSCARDCONTEXT phContext);
typedef LONG (WINAPI *PFNSCardFreeMemory)(SCARDCONTEXT hContext,LPCVOID pvMem);
typedef LONG (WINAPI *PFNSCardGetStatusChangeA)(SCARDCONTEXT hContext,DWORD dwTimeout,LPSCARD_READERSTATEA rgReaderStates,DWORD cReaders);
typedef LONG (WINAPI *PFNSCardGetStatusChangeW)(SCARDCONTEXT hContext,DWORD dwTimeout,LPSCARD_READERSTATEW rgReaderStates,DWORD cReaders);
typedef LONG (WINAPI *PFNSCardIsValidContext)(SCARDCONTEXT hContext);
typedef LONG (WINAPI *PFNSCardListReadersA)(SCARDCONTEXT hContext,LPCSTR mszGroups,LPSTR mszReaders,LPDWORD pcchReaders);
typedef LONG (WINAPI *PFNSCardListReadersW)(SCARDCONTEXT hContext,LPCWSTR mszGroups,LPWSTR mszReaders,LPDWORD pcchReaders);
typedef LONG (WINAPI *PFNSCardReleaseContext)(SCARDCONTEXT hContext);
typedef LONG (WINAPI *PFNSCardStatusA)(SCARDHANDLE hCard,LPSTR mszReaderNames,LPDWORD pcchReaderLen,LPDWORD pdwState,LPDWORD pdwProtocol,LPBYTE pbAtr,LPDWORD pcbAtrLen);
typedef LONG (WINAPI *PFNSCardStatusW)(SCARDHANDLE hCard,LPWSTR mszReaderNames,LPDWORD pcchReaderLen,LPDWORD pdwState,LPDWORD pdwProtocol,LPBYTE pbAtr,LPDWORD pcbAtrLen);
typedef LONG (WINAPI *PFNSCardTransmit)(SCARDHANDLE hCard,LPCSCARD_IO_REQUEST pioSendPci,LPCBYTE pbSendBuffer,DWORD cbSendLength,LPSCARD_IO_REQUEST pioRecvPci,LPBYTE pbRecvBuffer,LPDWORD pcbRecvLength);

static PFNSCardConnectA g_pfnSCardConnectA;
static PFNSCardConnectW g_pfnSCardConnectW;
static PFNSCardDisconnect g_pfnSCardDisconnect;
static PFNSCardEstablishContext g_pfnSCardEstablishContext;
static PFNSCardFreeMemory g_pfnSCardFreeMemory;
static PFNSCardGetStatusChangeA g_pfnSCardGetStatusChangeA;
static PFNSCardGetStatusChangeW g_pfnSCardGetStatusChangeW;
static PFNSCardIsValidContext g_pfnSCardIsValidContext;
static PFNSCardListReadersA g_pfnSCardListReadersA;
static PFNSCardListReadersW g_pfnSCardListReadersW;
static PFNSCardReleaseContext g_pfnSCardReleaseContext;
static PFNSCardStatusA g_pfnSCardStatusA;
static PFNSCardStatusW g_pfnSCardStatusW;
static PFNSCardTransmit g_pfnSCardTransmit;
static SCARD_IO_REQUEST *g_prgSCardT1Pci;
static HMODULE g_hWrapped;
static HANDLE g_hHeap;
static CRITICAL_SECTION g_section;
static LPVOID g_allocated[64];
static int g_allocatedCount;

static LPVOID AllocMemory(DWORD cbSize)
{
    LPVOID pvRet = NULL;
    ::EnterCriticalSection(&g_section);
    if (g_allocatedCount < _countof(g_allocated)) {
        if ((g_allocated[g_allocatedCount] = ::HeapAlloc(g_hHeap, 0, cbSize)) != NULL) {
            pvRet = g_allocated[g_allocatedCount++];
        }
    }
    ::LeaveCriticalSection(&g_section);
    return pvRet;
}

static bool FreeMemory(LPVOID pvMem)
{
    bool fRet = false;
    ::EnterCriticalSection(&g_section);
    for (int i = 0; i < g_allocatedCount; ++i) {
        if (g_allocated[i] == pvMem) {
            ::HeapFree(g_hHeap, 0, g_allocated[i]);
            ::MoveMemory(g_allocated + i, g_allocated + i + 1, (g_allocatedCount - i - 1) * sizeof(LPVOID));
            --g_allocatedCount;
            fRet = true;
        }
    }
    ::LeaveCriticalSection(&g_section);
    // Returns true when pvMem was allocated by AllocMemory().
    return fRet;
}

extern "C" {
SCARD_IO_REQUEST g_rgSCardT1Pci_ = {SCARD_PROTOCOL_T1, 8};

LONG WINAPI SCardConnectA_(SCARDCONTEXT hContext,LPCSTR szReader,DWORD dwShareMode,DWORD dwPreferredProtocols,LPSCARDHANDLE phCard,LPDWORD pdwActiveProtocol)
{
    return g_pfnSCardConnectA(hContext,szReader,dwShareMode,dwPreferredProtocols,phCard,pdwActiveProtocol);
}
LONG WINAPI SCardConnectW_(SCARDCONTEXT hContext,LPCWSTR szReader,DWORD dwShareMode,DWORD dwPreferredProtocols,LPSCARDHANDLE phCard,LPDWORD pdwActiveProtocol)
{
    return g_pfnSCardConnectW(hContext,szReader,dwShareMode,dwPreferredProtocols,phCard,pdwActiveProtocol);
}
LONG WINAPI SCardDisconnect_(SCARDHANDLE hCard,DWORD dwDisposition)
{
    return g_pfnSCardDisconnect(hCard,dwDisposition);
}
LONG WINAPI SCardEstablishContext_(DWORD dwScope,LPCVOID pvReserved1,LPCVOID pvReserved2,LPSCARDCONTEXT phContext)
{
    return g_pfnSCardEstablishContext(dwScope,pvReserved1,pvReserved2,phContext);
}
LONG WINAPI SCardFreeMemory_(SCARDCONTEXT hContext,LPCVOID pvMem)
{
    if (FreeMemory(const_cast<LPVOID>(pvMem)) || !g_pfnSCardFreeMemory) {
        return SCARD_S_SUCCESS;
    }
    return g_pfnSCardFreeMemory(hContext,pvMem);
}
LONG WINAPI SCardGetStatusChangeA_(SCARDCONTEXT hContext,DWORD dwTimeout,LPSCARD_READERSTATEA rgReaderStates,DWORD cReaders)
{
    if (!g_pfnSCardGetStatusChangeA) {
        if (dwTimeout == 0 && cReaders == 1 && rgReaderStates->dwCurrentState == SCARD_STATE_UNAWARE) {
            rgReaderStates->dwEventState = SCARD_STATE_PRESENT;
            return SCARD_S_SUCCESS;
        }
        return SCARD_E_INVALID_PARAMETER;
    }
    return g_pfnSCardGetStatusChangeA(hContext,dwTimeout,rgReaderStates,cReaders);
}
LONG WINAPI SCardGetStatusChangeW_(SCARDCONTEXT hContext,DWORD dwTimeout,LPSCARD_READERSTATEW rgReaderStates,DWORD cReaders)
{
    if (!g_pfnSCardGetStatusChangeW) {
        if (dwTimeout == 0 && cReaders == 1 && rgReaderStates->dwCurrentState == SCARD_STATE_UNAWARE) {
            rgReaderStates->dwEventState = SCARD_STATE_PRESENT;
            return SCARD_S_SUCCESS;
        }
        return SCARD_E_INVALID_PARAMETER;
    }
    return g_pfnSCardGetStatusChangeW(hContext,dwTimeout,rgReaderStates,cReaders);
}
LONG WINAPI SCardIsValidContext_(SCARDCONTEXT hContext)
{
    if (!g_pfnSCardIsValidContext) {
        return hContext ? SCARD_S_SUCCESS : ERROR_INVALID_HANDLE;
    }
    return g_pfnSCardIsValidContext(hContext);
}
LONG WINAPI SCardListReadersA_(SCARDCONTEXT hContext,LPCSTR mszGroups,LPSTR mszReaders,LPDWORD pcchReaders)
{
    if (mszReaders && *pcchReaders == SCARD_AUTOALLOCATE) {
        *pcchReaders = 0;
        LONG rv = g_pfnSCardListReadersA(hContext,mszGroups,NULL,pcchReaders);
        if (rv != SCARD_S_SUCCESS) return rv;
        if ((*(LPSTR*)mszReaders = (LPSTR)AllocMemory(*pcchReaders * sizeof(char))) == NULL) return SCARD_E_NO_MEMORY;
        return g_pfnSCardListReadersA(hContext,mszGroups,*(LPSTR*)mszReaders,pcchReaders);
    }
    return g_pfnSCardListReadersA(hContext,mszGroups,mszReaders,pcchReaders);
}
LONG WINAPI SCardListReadersW_(SCARDCONTEXT hContext,LPCWSTR mszGroups,LPWSTR mszReaders,LPDWORD pcchReaders)
{
    if (mszReaders && *pcchReaders == SCARD_AUTOALLOCATE) {
        *pcchReaders = 0;
        LONG rv = g_pfnSCardListReadersW(hContext,mszGroups,NULL,pcchReaders);
        if (rv != SCARD_S_SUCCESS) return rv;
        if ((*(LPWSTR*)mszReaders = (LPWSTR)AllocMemory(*pcchReaders * sizeof(WCHAR))) == NULL) return SCARD_E_NO_MEMORY;
        return g_pfnSCardListReadersW(hContext,mszGroups,*(LPWSTR*)mszReaders,pcchReaders);
    }
    return g_pfnSCardListReadersW(hContext,mszGroups,mszReaders,pcchReaders);
}
LONG WINAPI SCardReleaseContext_(SCARDCONTEXT hContext)
{
    return g_pfnSCardReleaseContext(hContext);
}
LONG WINAPI SCardStatusA_(SCARDHANDLE hCard,LPSTR mszReaderNames,LPDWORD pcchReaderLen,LPDWORD pdwState,LPDWORD pdwProtocol,LPBYTE pbAtr,LPDWORD pcbAtrLen)
{
    if (!g_pfnSCardStatusA) {
        if (pcchReaderLen) {
            if (!mszReaderNames || *pcchReaderLen != SCARD_AUTOALLOCATE) return SCARD_E_INVALID_PARAMETER;
            for (*pcchReaderLen = 2; READER_NAME_A[*pcchReaderLen-2] || READER_NAME_A[*pcchReaderLen-1]; ++*pcchReaderLen);
            if ((*(LPSTR*)mszReaderNames = (LPSTR)AllocMemory(sizeof(READER_NAME_A))) == NULL) return SCARD_E_NO_MEMORY;
            ::CopyMemory(*(LPSTR*)mszReaderNames, READER_NAME_A, sizeof(READER_NAME_A));
        }
        if (pdwState) *pdwState = SCARD_PRESENT;
        if (pdwProtocol) *pdwProtocol = SCARD_PROTOCOL_T1;
        if (pcbAtrLen) {
            if (*pcbAtrLen == SCARD_AUTOALLOCATE) return SCARD_E_INVALID_PARAMETER;
            *pcbAtrLen = 0;
        }
        return SCARD_S_SUCCESS;
    }
    return g_pfnSCardStatusA(hCard,mszReaderNames,pcchReaderLen,pdwState,pdwProtocol,pbAtr,pcbAtrLen);
}
LONG WINAPI SCardStatusW_(SCARDHANDLE hCard,LPWSTR mszReaderNames,LPDWORD pcchReaderLen,LPDWORD pdwState,LPDWORD pdwProtocol,LPBYTE pbAtr,LPDWORD pcbAtrLen)
{
    if (!g_pfnSCardStatusW) {
        if (pcchReaderLen) {
            if (!mszReaderNames || *pcchReaderLen != SCARD_AUTOALLOCATE) return SCARD_E_INVALID_PARAMETER;
            for (*pcchReaderLen = 2; READER_NAME_W[*pcchReaderLen-2] || READER_NAME_W[*pcchReaderLen-1]; ++*pcchReaderLen);
            if ((*(LPWSTR*)mszReaderNames = (LPWSTR)AllocMemory(sizeof(READER_NAME_W))) == NULL) return SCARD_E_NO_MEMORY;
            ::CopyMemory(*(LPWSTR*)mszReaderNames, READER_NAME_W, sizeof(READER_NAME_W));
        }
        if (pdwState) *pdwState = SCARD_PRESENT;
        if (pdwProtocol) *pdwProtocol = SCARD_PROTOCOL_T1;
        if (pcbAtrLen) {
            if (*pcbAtrLen == SCARD_AUTOALLOCATE) return SCARD_E_INVALID_PARAMETER;
            *pcbAtrLen = 0;
        }
        return SCARD_S_SUCCESS;
    }
    return g_pfnSCardStatusW(hCard,mszReaderNames,pcchReaderLen,pdwState,pdwProtocol,pbAtr,pcbAtrLen);
}
LONG WINAPI SCardTransmit_(SCARDHANDLE hCard,LPCSCARD_IO_REQUEST pioSendPci,LPCBYTE pbSendBuffer,DWORD cbSendLength,LPSCARD_IO_REQUEST pioRecvPci,LPBYTE pbRecvBuffer,LPDWORD pcbRecvLength)
{
    if (pioSendPci == &g_rgSCardT1Pci_) pioSendPci = g_prgSCardT1Pci;
    LONG rv = g_pfnSCardTransmit(hCard,pioSendPci,pbSendBuffer,cbSendLength,pioRecvPci,pbRecvBuffer,pcbRecvLength);
    if (rv == SCARD_S_SUCCESS && pbRecvBuffer) {
        // Response corrections
        // "Card ID Information Acquire Response"
        if (cbSendLength==5 && pbSendBuffer[0]==0x90 && pbSendBuffer[1]==0x32 && pbSendBuffer[2]==0x00 && pbSendBuffer[3]==0x00 && pbSendBuffer[4]==0x00) {
            // Empty "Card ID"
            if (*pcbRecvLength >= 19 && !pbRecvBuffer[9] && !pbRecvBuffer[10] && !pbRecvBuffer[11] && !pbRecvBuffer[12] && !pbRecvBuffer[13] && !pbRecvBuffer[14]) {
                static const BYTE bCardID[6] = { 1, 2, 3, 4, 5, 6 };
                ::CopyMemory(pbRecvBuffer + 9, bCardID, 6);
            }
        }
    }
    return rv;
}
}

#define CHECK_NULL(x) { if ((x) == NULL) return FALSE; }

BOOL WINAPI DllMain(HINSTANCE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        CHECK_NULL( g_hWrapped = ::LoadLibrary(WRAPPED_NAME) );
        CHECK_NULL( g_pfnSCardConnectA = (PFNSCardConnectA)::GetProcAddress(g_hWrapped, "SCardConnectA") );
        CHECK_NULL( g_pfnSCardConnectW = (PFNSCardConnectW)::GetProcAddress(g_hWrapped, "SCardConnectW") );
        CHECK_NULL( g_pfnSCardDisconnect = (PFNSCardDisconnect)::GetProcAddress(g_hWrapped, "SCardDisconnect") );
        CHECK_NULL( g_pfnSCardEstablishContext = (PFNSCardEstablishContext)::GetProcAddress(g_hWrapped, "SCardEstablishContext") );
        g_pfnSCardFreeMemory = (PFNSCardFreeMemory)::GetProcAddress(g_hWrapped, "SCardFreeMemory");
        g_pfnSCardGetStatusChangeA = (PFNSCardGetStatusChangeA)::GetProcAddress(g_hWrapped, "SCardGetStatusChangeA");
        g_pfnSCardGetStatusChangeW = (PFNSCardGetStatusChangeW)::GetProcAddress(g_hWrapped, "SCardGetStatusChangeW");
        g_pfnSCardIsValidContext = (PFNSCardIsValidContext)::GetProcAddress(g_hWrapped, "SCardIsValidContext");
        CHECK_NULL( g_pfnSCardListReadersA = (PFNSCardListReadersA)::GetProcAddress(g_hWrapped, "SCardListReadersA") );
        CHECK_NULL( g_pfnSCardListReadersW = (PFNSCardListReadersW)::GetProcAddress(g_hWrapped, "SCardListReadersW") );
        CHECK_NULL( g_pfnSCardReleaseContext = (PFNSCardReleaseContext)::GetProcAddress(g_hWrapped, "SCardReleaseContext") );
        g_pfnSCardStatusA = (PFNSCardStatusA)::GetProcAddress(g_hWrapped, "SCardStatusA");
        g_pfnSCardStatusW = (PFNSCardStatusW)::GetProcAddress(g_hWrapped, "SCardStatusW");
        CHECK_NULL( g_pfnSCardTransmit = (PFNSCardTransmit)::GetProcAddress(g_hWrapped, "SCardTransmit") );
        CHECK_NULL( g_prgSCardT1Pci = (SCARD_IO_REQUEST*)::GetProcAddress(g_hWrapped, "g_rgSCardT1Pci") );
        CHECK_NULL( g_hHeap = ::HeapCreate(0, 0, 0) );
        ::InitializeCriticalSection(&g_section);
        g_allocatedCount = 0;
        break;
    case DLL_PROCESS_DETACH:
        ::DeleteCriticalSection(&g_section);
        ::HeapDestroy(g_hHeap);
        ::FreeLibrary(g_hWrapped);
        break;
    }
    return TRUE;
}
