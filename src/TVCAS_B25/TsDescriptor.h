// TsDescriptor.h: �L�q�q���b�p�[�N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include <Vector>
#include "Common.h"
#include "MediaData.h"


/////////////////////////////////////////////////////////////////////////////
// �L�q�q�̊��N���X
/////////////////////////////////////////////////////////////////////////////

class CBaseDesc
{
public:
	CBaseDesc();
	CBaseDesc(const CBaseDesc &Operand);
	virtual ~CBaseDesc();
	CBaseDesc & operator = (const CBaseDesc &Operand);

	virtual void CopyDesc(const CBaseDesc *pOperand);
	const bool ParseDesc(const BYTE *pHexData, const WORD wDataLength);

	const bool IsValid(void) const;
	const BYTE GetTag(void) const;
	const BYTE GetLength(void) const;

	virtual void Reset(void);

protected:
	virtual const bool StoreContents(const BYTE *pPayload);

	BYTE m_byDescTag;	// �L�q�q�^�O
	BYTE m_byDescLen;	// �L�q�q��
	bool m_bIsValid;	// ��͌���
};


/////////////////////////////////////////////////////////////////////////////
// [0x09] Conditional Access Method �L�q�q���ۉ��N���X
/////////////////////////////////////////////////////////////////////////////

class CCaMethodDesc : public CBaseDesc
{
public:
	enum {DESC_TAG = 0x09U};

	CCaMethodDesc();
	CCaMethodDesc(const CCaMethodDesc &Operand);
	CCaMethodDesc & operator = (const CCaMethodDesc &Operand);

// CBaseDesc
	virtual void CopyDesc(const CBaseDesc *pOperand);
	virtual void Reset(void);

// CCaMethodDesc
	const WORD GetCaMethodID(void) const;
	const WORD GetCaPID(void) const;
	const CMediaData * GetPrivateData(void) const;

protected:
	virtual const bool StoreContents(const BYTE *pPayload);

	WORD m_wCaMethodID;			// Conditional Access Method ID
	WORD m_wCaPID;				// Conditional Access PID
	CMediaData m_PrivateData;	// Private Data
};


/////////////////////////////////////////////////////////////////////////////
// [0x48] Service �L�q�q���ۉ��N���X
/////////////////////////////////////////////////////////////////////////////

class CServiceDesc : public CBaseDesc
{
public:
	enum {DESC_TAG = 0x48U};

	CServiceDesc();
	CServiceDesc(const CServiceDesc &Operand);
	CServiceDesc & operator = (const CServiceDesc &Operand);
	
// CBaseDesc
	virtual void CopyDesc(const CBaseDesc *pOperand);
	virtual void Reset(void);

// CServiceDesc
	const BYTE GetServiceType(void) const;
	const DWORD GetProviderName(LPTSTR lpszDst, int MaxLength) const;
	const DWORD GetServiceName(LPTSTR lpszDst, int MaxLength) const;

protected:
	virtual const bool StoreContents(const BYTE *pPayload);

	BYTE m_byServiceType;			// Service Type
	TCHAR m_szProviderName[256];	// Service Provider Name
	TCHAR m_szServiceName[256];		// Service Name
};


/////////////////////////////////////////////////////////////////////////////
// [0x4D] Short Event �L�q�q���ۉ��N���X
/////////////////////////////////////////////////////////////////////////////

class CShortEventDesc : public CBaseDesc
{
public:
	enum {DESC_TAG = 0x4DU};

	CShortEventDesc();
	CShortEventDesc(const CShortEventDesc &Operand);
	CShortEventDesc & operator = (const CShortEventDesc &Operand);

// CBaseDesc
	virtual void CopyDesc(const CBaseDesc *pOperand);
	virtual void Reset(void);

// CShortEventDesc
	const DWORD GetLanguageCode(void) const;
	const DWORD GetEventName(LPTSTR lpszDst, int MaxLength) const;
	const DWORD GetEventDesc(LPTSTR lpszDst, int MaxLength) const;

protected:
	virtual const bool StoreContents(const BYTE *pPayload);

	DWORD m_dwLanguageCode;			// ISO639  Language Code
	TCHAR m_szEventName[256];		// Event Name
	TCHAR m_szEventDesc[256];		// Event Description
};


/////////////////////////////////////////////////////////////////////////////
// [0x4E] Extended Event �L�q�q���ۉ��N���X
/////////////////////////////////////////////////////////////////////////////

class CExtendedEventDesc : public CBaseDesc
{
public:
	enum {DESC_TAG = 0x4EU};

	CExtendedEventDesc();
	CExtendedEventDesc(const CExtendedEventDesc &Operand);
	CExtendedEventDesc & operator = (const CExtendedEventDesc &Operand);

// CBaseDesc
	virtual void CopyDesc(const CBaseDesc *pOperand);
	virtual void Reset(void);

// CExtendedEventDesc
	enum {
		MAX_DESCRIPTION = 32
	};
	struct ItemInfo {
		TCHAR szDescription[MAX_DESCRIPTION];
		BYTE ItemLength;
		BYTE ItemChar[220];
	};

	BYTE GetDescriptorNumber() const;
	BYTE GetLastDescriptorNumber() const;
	DWORD GetLanguageCode() const;
	int GetItemCount() const;
	const ItemInfo * GetItem(int Index) const;

protected:
	virtual const bool StoreContents(const BYTE *pPayload);

	BYTE m_DescriptorNumber;
	BYTE m_LastDescriptorNumber;
	DWORD m_LanguageCode;			// ISO639  Language Code
	std::vector<ItemInfo> m_ItemList;
};


/////////////////////////////////////////////////////////////////////////////
// [0x52] Stream Identifier �L�q�q���ۉ��N���X
/////////////////////////////////////////////////////////////////////////////

class CStreamIdDesc : public CBaseDesc
{
public:
	enum {DESC_TAG = 0x52U};

	CStreamIdDesc();
	CStreamIdDesc(const CStreamIdDesc &Operand);
	CStreamIdDesc & operator = (const CStreamIdDesc &Operand);

// CBaseDesc
	virtual void CopyDesc(const CBaseDesc *pOperand);
	virtual void Reset(void);

// CStreamIdDesc
	const BYTE GetComponentTag(void) const;

protected:
	virtual const bool StoreContents(const BYTE *pPayload);

	BYTE m_byComponentTag;		// Component Tag
};


/////////////////////////////////////////////////////////////////////////////
// [0x40] Network Name �L�q�q���ۉ��N���X
/////////////////////////////////////////////////////////////////////////////

class CNetworkNameDesc : public CBaseDesc
{
public:
	enum {DESC_TAG = 0x40U};

	CNetworkNameDesc();
	CNetworkNameDesc(const CNetworkNameDesc &Operand);
	CNetworkNameDesc & operator = (const CNetworkNameDesc &Operand);

// CBaseDesc
	virtual void CopyDesc(const CBaseDesc *pOperand);
	virtual void Reset(void);

// CNetworkNameDesc
	const DWORD GetNetworkName(LPTSTR pszName, int MaxLength) const;

protected:
	virtual const bool StoreContents(const BYTE *pPayload);

	TCHAR m_szNetworkName[32];
};


/////////////////////////////////////////////////////////////////////////////
// [0x41] Service List �L�q�q���ۉ��N���X
/////////////////////////////////////////////////////////////////////////////

class CServiceListDesc : public CBaseDesc
{
public:
	enum {DESC_TAG = 0x41U};

	CServiceListDesc();
	CServiceListDesc(const CServiceListDesc &Operand);
	CServiceListDesc & operator = (const CServiceListDesc &Operand);

// CBaseDesc
	virtual void CopyDesc(const CBaseDesc *pOperand);
	virtual void Reset(void);

// CServiceListDesc
	struct ServiceInfo {
		WORD ServiceID;
		BYTE ServiceType;
	};

	const int GetServiceNum() const;
	const int GetServiceIndexByID(const WORD ServiceID) const;
	const BYTE GetServiceTypeByID(const WORD ServiceID) const;
	const bool GetServiceInfo(const int Index, ServiceInfo *pInfo) const;

protected:
	virtual const bool StoreContents(const BYTE *pPayload);

	std::vector<ServiceInfo> m_ServiceList;
};


/////////////////////////////////////////////////////////////////////////////
// [0x43] Satellite Delivery System �L�q�q���ۉ��N���X
/////////////////////////////////////////////////////////////////////////////

class CSatelliteDeliverySystemDesc : public CBaseDesc
{
public:
	enum {DESC_TAG = 0x43U};

	CSatelliteDeliverySystemDesc();
	CSatelliteDeliverySystemDesc(const CSatelliteDeliverySystemDesc &Operand);
	CSatelliteDeliverySystemDesc & operator = (const CSatelliteDeliverySystemDesc &Operand);

// CBaseDesc
	virtual void CopyDesc(const CBaseDesc *pOperand);
	virtual void Reset(void);

// CSatelliteDeliverySystemDesc
	const DWORD GetFrequency() const;
	const WORD GetOrbitalPosition() const;
	const bool GetWestEastFlag() const;
	const BYTE GetPolarization() const;
	const BYTE GetModulation() const;
	const DWORD GetSymbolRate() const;
	const BYTE GetFECInner() const;

protected:
	virtual const bool StoreContents(const BYTE *pPayload);

	DWORD m_Frequency;
	WORD m_OrbitalPosition;
	bool m_bWestEastFlag;
	BYTE m_Polarization;
	BYTE m_Modulation;
	DWORD m_SymbolRate;
	BYTE m_FECInner;
};


/////////////////////////////////////////////////////////////////////////////
// [0xFA] Terrestrial Delivery System �L�q�q���ۉ��N���X
/////////////////////////////////////////////////////////////////////////////

class CTerrestrialDeliverySystemDesc : public CBaseDesc
{
public:
	enum {DESC_TAG = 0xFAU};

	CTerrestrialDeliverySystemDesc();
	CTerrestrialDeliverySystemDesc(const CTerrestrialDeliverySystemDesc &Operand);
	CTerrestrialDeliverySystemDesc & operator = (const CTerrestrialDeliverySystemDesc &Operand);

// CBaseDesc
	virtual void CopyDesc(const CBaseDesc *pOperand);
	virtual void Reset(void);

// CTerrestrialDeliverySystemDesc
	const WORD GetAreaCode() const;
	const BYTE GetGuardInterval() const;
	const BYTE GetTransmissionMode() const;
	const int GetFrequencyNum() const;
	const WORD GetFrequency(const int Index) const;

protected:
	virtual const bool StoreContents(const BYTE *pPayload);

	WORD m_AreaCode;
	BYTE m_GuardInterval;
	BYTE m_TransmissionMode;
	std::vector<WORD> m_Frequency;
};


/////////////////////////////////////////////////////////////////////////////
// [0xFE] System Management �L�q�q���ۉ��N���X
/////////////////////////////////////////////////////////////////////////////

class CSystemManageDesc : public CBaseDesc
{
public:
	enum {DESC_TAG = 0xFEU};

	CSystemManageDesc();
	CSystemManageDesc(const CSystemManageDesc &Operand);
	CSystemManageDesc & operator = (const CSystemManageDesc &Operand);

// CBaseDesc
	virtual void CopyDesc(const CBaseDesc *pOperand);
	virtual void Reset(void);

// CServiceDesc
	const BYTE GetBroadcastingFlag(void) const;
	const BYTE GetBroadcastingID(void) const;
	const BYTE GetAdditionalBroadcastingID(void) const;

protected:
	virtual const bool StoreContents(const BYTE *pPayload);

	BYTE m_byBroadcastingFlag;
	BYTE m_byBroadcastingID;
	BYTE m_byAdditionalBroadcastingID;
};


/////////////////////////////////////////////////////////////////////////////
// [0xCD] TS Information �L�q�q���ۉ��N���X
/////////////////////////////////////////////////////////////////////////////

class CTSInfoDesc : public CBaseDesc
{
public:
	enum {DESC_TAG = 0xCDU};

	CTSInfoDesc();
	CTSInfoDesc(const CTSInfoDesc &Operand);
	CTSInfoDesc & operator = (const CTSInfoDesc &Operand);

// CBaseDesc
	virtual void CopyDesc(const CBaseDesc *pOperand);
	virtual void Reset(void);

// CTSInfoDesc
	const BYTE GetRemoteControlKeyID(void) const;
	const DWORD GetTSName(LPTSTR pszName, int MaxLength) const;

protected:
	virtual const bool StoreContents(const BYTE *pPayload);

	BYTE m_byRemoteControlKeyID;
	TCHAR m_szTSName[32];
};


/////////////////////////////////////////////////////////////////////////////
// [0x50] Component �L�q�q���ۉ��N���X
/////////////////////////////////////////////////////////////////////////////

class CComponentDesc : public CBaseDesc
{
public:
	enum {DESC_TAG = 0x50U};

	CComponentDesc();
	CComponentDesc(const CComponentDesc &Operand);
	CComponentDesc & operator = (const CComponentDesc &Operand);

// CBaseDesc
	virtual void CopyDesc(const CBaseDesc *pOperand);
	virtual void Reset(void);

// CComponentDesc
	const BYTE GetStreamContent(void) const;
	const BYTE GetComponentType(void) const;
	const BYTE GetComponentTag(void) const;
	const DWORD GetLanguageCode(void) const;
	const DWORD GetText(LPTSTR pszText, int MaxLength) const;

protected:
	virtual const bool StoreContents(const BYTE *pPayload);

	BYTE m_StreamContent;
	BYTE m_ComponentType;
	BYTE m_ComponentTag;
	DWORD m_LanguageCode;
	TCHAR m_szText[64];
};


/////////////////////////////////////////////////////////////////////////////
// [0xC4] Audio Component �L�q�q���ۉ��N���X
/////////////////////////////////////////////////////////////////////////////

class CAudioComponentDesc : public CBaseDesc
{
public:
	enum {DESC_TAG = 0xC4U};

	CAudioComponentDesc();
	CAudioComponentDesc(const CAudioComponentDesc &Operand);
	CAudioComponentDesc & operator = (const CAudioComponentDesc &Operand);

// CBaseDesc
	virtual void CopyDesc(const CBaseDesc *pOperand);
	virtual void Reset(void);

// CAudioComponentDesc
	const BYTE GetStreamContent(void) const;
	const BYTE GetComponentType(void) const;
	const BYTE GetComponentTag(void) const;
	const BYTE GetSimulcastGroupTag(void) const;
	const bool GetESMultiLingualFlag(void) const;
	const bool GetMainComponentFlag(void) const;
	const BYTE GetQualityIndicator(void) const;
	const BYTE GetSamplingRate(void) const;
	const DWORD GetLanguageCode(void) const;
	const DWORD GetLanguageCode2(void) const;
	const DWORD GetText(LPTSTR pszText, int MaxLength) const;

protected:
	virtual const bool StoreContents(const BYTE *pPayload);

	BYTE m_StreamContent;
	BYTE m_ComponentType;
	BYTE m_ComponentTag;
	BYTE m_StreamType;
	BYTE m_SimulcastGroupTag;
	bool m_bESMultiLingualFlag;
	bool m_bMainComponentFlag;
	BYTE m_QualityIndicator;
	BYTE m_SamplingRate;
	DWORD m_LanguageCode;
	DWORD m_LanguageCode2;
	TCHAR m_szText[64];
};


/////////////////////////////////////////////////////////////////////////////
// [0x54] Content �L�q�q���ۉ��N���X
/////////////////////////////////////////////////////////////////////////////

class CContentDesc : public CBaseDesc
{
public:
	enum {DESC_TAG = 0x54U};

	CContentDesc();
	CContentDesc(const CContentDesc &Operand);
	CContentDesc & operator = (const CContentDesc &Operand);

// CBaseDesc
	virtual void CopyDesc(const CBaseDesc *pOperand);
	virtual void Reset(void);

// CContentDesc
	struct Nibble {
		BYTE ContentNibbleLevel1;
		BYTE ContentNibbleLevel2;
		BYTE UserNibble1;
		BYTE UserNibble2;

		bool operator==(const Nibble &Operand) const {
			return ContentNibbleLevel1 == Operand.ContentNibbleLevel1
				&& ContentNibbleLevel2 == Operand.ContentNibbleLevel2
				&& UserNibble1 == Operand.UserNibble1
				&& UserNibble2 == Operand.UserNibble2;
		}
		bool operator!=(const Nibble &Operand) const { return !(*this==Operand); }
	};

	int GetNibbleCount() const;
	bool GetNibble(int Index, Nibble *pNibble) const;

protected:
	virtual const bool StoreContents(const BYTE *pPayload);

	int m_NibbleCount;
	Nibble m_NibbleList[7];
};


/////////////////////////////////////////////////////////////////////////////
// [0xCF] Logo Transmission �L�q�q���ۉ��N���X
/////////////////////////////////////////////////////////////////////////////

class CLogoTransmissionDesc : public CBaseDesc
{
public:
	enum {DESC_TAG = 0xCFU};

	CLogoTransmissionDesc();
	CLogoTransmissionDesc(const CLogoTransmissionDesc &Operand);
	CLogoTransmissionDesc & operator = (const CLogoTransmissionDesc &Operand);

// CBaseDesc
	virtual void CopyDesc(const CBaseDesc *pOperand);
	virtual void Reset(void);

// CLogoTransmissionDesc
	// logo_transmission_type
	enum {
		TRANSMISSION_UNDEFINED,
		TRANSMISSION_CDT1,		// CDT�`������1
		TRANSMISSION_CDT2,		// CDT�`������2
		TRANSMISSION_CHAR		// �ȈՃ��S����
	};
	enum {
		MAX_LOGO_CHAR		= 12,		// �ő�ȈՃ��S��
		LOGOID_INVALID		= 0xFFFF,	// ������ logo_id
		LOGOVERSION_INVALID	= 0xFFFF,	// ������ logo_version
		DATAID_INVALID		= 0xFFFF	// ������ download_data_id
	};

	BYTE GetLogoTransmissionType() const;
	WORD GetLogoID() const;
	WORD GetLogoVersion() const;
	WORD GetDownloadDataID() const;
	int GetLogoChar(char *pChar, int MaxLength) const;

protected:
	virtual const bool StoreContents(const BYTE *pPayload);

	BYTE m_LogoTransmissionType;	// logo_transmission_type
	WORD m_LogoID;					// logo_id
	WORD m_LogoVersion;				// logo_version
	WORD m_DownloadDataID;			// download_data_id
	char m_LogoChar[MAX_LOGO_CHAR];	// logo_char
};


/////////////////////////////////////////////////////////////////////////////
// [0xD5] Series �L�q�q���ۉ��N���X
/////////////////////////////////////////////////////////////////////////////

class CSeriesDesc : public CBaseDesc
{
public:
	enum {DESC_TAG = 0xD5U};

	CSeriesDesc();
	CSeriesDesc(const CSeriesDesc &Operand);
	CSeriesDesc & operator = (const CSeriesDesc &Operand);

// CBaseDesc
	virtual void CopyDesc(const CBaseDesc *pOperand);
	virtual void Reset(void);

// CSeriesDesc
	enum {
		SERIESID_INVALID	= 0xFFFF,
		MAX_SERIES_NAME		= 64
	};
	enum {
		PROGRAMPATTERN_IRREGULAR,					// �s���
		PROGRAMPATTERN_ACROSS_THE_BOARD,			// �єԑg
		PROGRAMPATTERN_WEEKLY,						// �T���
		PROGRAMPATTERN_MONTHLY,						// �����
		PROGRAMPATTERN_MULTIPLE_EPISODES_IN_DAY,	// �������ɕ����b�Ґ�
		PROGRAMPATTERN_DIVISION_LONG_PROGRAM,		// �����Ԕԑg�̕���
		PROGRAMPATTERN_INVALID = 0xFF
	};

	WORD GetSeriesID() const;
	BYTE GetRepeatLabel() const;
	BYTE GetProgramPattern() const;
	bool IsExpireDateValid() const;
	bool GetExpireDate(SYSTEMTIME *pDate) const;
	WORD GetEpisodeNumber() const;
	WORD GetLastEpisodeNumber() const;
	int GetSeriesName(LPTSTR pszName, int MaxName) const;

protected:
	virtual const bool StoreContents(const BYTE *pPayload);

	WORD m_SeriesID;
	BYTE m_RepeatLabel;
	BYTE m_ProgramPattern;
	bool m_bExpireDateValidFlag;
	SYSTEMTIME m_ExpireDate;
	WORD m_EpisodeNumber;
	WORD m_LastEpisodeNumber;
	TCHAR m_szSeriesName[MAX_SERIES_NAME];
};


/////////////////////////////////////////////////////////////////////////////
// [0xD6] Event Group �L�q�q���ۉ��N���X
/////////////////////////////////////////////////////////////////////////////

class CEventGroupDesc : public CBaseDesc
{
public:
	enum {DESC_TAG = 0xD6U};

	CEventGroupDesc();
	CEventGroupDesc(const CEventGroupDesc &Operand);
	CEventGroupDesc & operator = (const CEventGroupDesc &Operand);

// CBaseDesc
	virtual void CopyDesc(const CBaseDesc *pOperand);
	virtual void Reset(void);

// CEventGroupDesc
	enum {
		GROUPTYPE_UNDEFINED,
		GROUPTYPE_COMMON,
		GROUPTYPE_RELAY,
		GROUPTYPE_MOVEMENT,
		GROUPTYPE_RELAY_TO_OTHER_NETWORK,
		GROUPTYPE_MOVEMENT_FROM_OTHER_NETWORK
	};

	struct EventInfo {
		WORD ServiceID;
		WORD EventID;
		WORD OriginalNetworkID;
		WORD TransportStreamID;

		bool operator==(const EventInfo &Operand) const
		{
			return ServiceID == Operand.ServiceID
				&& EventID == Operand.EventID
				&& OriginalNetworkID == Operand.OriginalNetworkID
				&& TransportStreamID == Operand.TransportStreamID;
		}

		bool operator!=(const EventInfo &Operand) const
		{
			return !(*this==Operand);
		}
	};

	BYTE GetGroupType() const;
	int GetEventNum() const;
	bool GetEventInfo(int Index, EventInfo *pInfo) const;

protected:
	virtual const bool StoreContents(const BYTE *pPayload);

	BYTE m_GroupType;
	std::vector<EventInfo> m_EventList;
};


/////////////////////////////////////////////////////////////////////////////
// [0x58] Local Time Offset �L�q�q���ۉ��N���X
/////////////////////////////////////////////////////////////////////////////

class CLocalTimeOffsetDesc : public CBaseDesc
{
public:
	enum {DESC_TAG = 0x58U};

	CLocalTimeOffsetDesc();
	CLocalTimeOffsetDesc(const CLocalTimeOffsetDesc &Operand);
	CLocalTimeOffsetDesc & operator = (const CLocalTimeOffsetDesc &Operand);

// CBaseDesc
	virtual void CopyDesc(const CBaseDesc *pOperand);
	virtual void Reset(void);

// CLocalTimeOffsetDesc
	enum {
		COUNTRYCODE_JPN		= 0x4A504EUL,
		COUNTRYREGION_ALL	= 0x00
	};

	bool IsValid() const;
	DWORD GetCountryCode() const;
	BYTE GetCountryRegionID() const;
	int GetLocalTimeOffset() const;
	bool GetTimeOfChange(SYSTEMTIME *pTime) const;
	int GetNextTimeOffset() const;

protected:
	virtual const bool StoreContents(const BYTE *pPayload);

	struct TimeOffsetInfo {
		bool bValid;
		DWORD CountryCode;
		BYTE CountryRegionID;
		BYTE LocalTimeOffsetPolarity;
		WORD LocalTimeOffset;
		SYSTEMTIME TimeOfChange;
		WORD NextTimeOffset;
	};
	TimeOffsetInfo m_Info;
};


/////////////////////////////////////////////////////////////////////////////
// [0xC9] Download Content �L�q�q���ۉ��N���X
/////////////////////////////////////////////////////////////////////////////

class CDownloadContentDesc : public CBaseDesc
{
public:
	enum {DESC_TAG = 0xC9U};

	CDownloadContentDesc();
	CDownloadContentDesc(const CDownloadContentDesc &Operand);
	CDownloadContentDesc & operator = (const CDownloadContentDesc &Operand);

// CBaseDesc
	virtual void CopyDesc(const CBaseDesc *pOperand);
	virtual void Reset(void);

// CDownloadContentDesc
	bool GetReboot() const;
	bool GetAddOn() const;
	DWORD GetComponentSize() const;
	DWORD GetDownloadID() const;
	DWORD GetTimeOutValueDII() const;
	DWORD GetLeakRate() const;
	BYTE GetComponentTag() const;

protected:
	virtual const bool StoreContents(const BYTE *pPayload);

	struct DownloadContentInfo
	{
		bool bReboot;
		bool bAddOn;
		bool bCompatibilityFlag;
		bool bModuleInfoFlag;
		bool bTextInfoFlag;
		DWORD ComponentSize;
		DWORD DownloadID;
		DWORD TimeOutValueDII;
		DWORD LeakRate;
		BYTE ComponentTag;
	};

	DownloadContentInfo m_Info;
};


/////////////////////////////////////////////////////////////////////////////
// [0xCB] CA Contract Info �L�q�q���ۉ��N���X
/////////////////////////////////////////////////////////////////////////////

class CCaContractInfoDesc : public CBaseDesc
{
public:
	enum {DESC_TAG = 0xCBU};

	CCaContractInfoDesc();
	CCaContractInfoDesc(const CCaContractInfoDesc &Operand);
	CCaContractInfoDesc & operator = (const CCaContractInfoDesc &Operand);

// CBaseDesc
	virtual void CopyDesc(const CBaseDesc *pOperand);
	virtual void Reset(void);

// CCAContractInfoDesc
	enum {
		MAX_NUM_OF_COMPONENT = 12,
		MAX_VERIFICATION_INFO_LENGTH = 172,
		MAX_FEE_NAME = 256
	};

	WORD GetCaSystemID() const;
	BYTE GetCaUnitID() const;
	BYTE GetNumOfComponent() const;
	BYTE GetComponentTag(BYTE Index) const;
	BYTE GetContractVerificationInfoLength() const;
	BYTE GetContractVerificationInfo(BYTE *pInfo, BYTE MaxLength) const;
	int GetFeeName(LPTSTR pszName, int MaxName) const;

protected:
	virtual const bool StoreContents(const BYTE *pPayload);

	WORD m_CaSystemID;
	BYTE m_CaUnitID;
	BYTE m_NumOfComponent;
	BYTE m_ComponentTag[MAX_NUM_OF_COMPONENT];
	BYTE m_ContractVerificationInfoLength;
	BYTE m_ContractVerificationInfo[MAX_VERIFICATION_INFO_LENGTH];
	TCHAR m_szFeeName[MAX_FEE_NAME];
};


/////////////////////////////////////////////////////////////////////////////
// �L�q�q�u���b�N���ۉ��N���X
/////////////////////////////////////////////////////////////////////////////

class CDescBlock
{
public:
	CDescBlock();
	CDescBlock(const CDescBlock &Operand);
	~CDescBlock();
	CDescBlock & operator = (const CDescBlock &Operand);

	const WORD ParseBlock(const BYTE *pHexData, const WORD wDataLength);
	const CBaseDesc * ParseBlock(const BYTE *pHexData, const WORD wDataLength, const BYTE byTag);

	virtual void Reset(void);

	const WORD GetDescNum(void) const;
	const CBaseDesc * GetDescByIndex(const WORD wIndex = 0U) const;
	const CBaseDesc * GetDescByTag(const BYTE byTag) const;

protected:
	CBaseDesc * ParseDesc(const BYTE *pHexData, const WORD wDataLength);
	static CBaseDesc * CreateDescInstance(const BYTE byTag);

	std::vector<CBaseDesc *> m_DescArray;
};
