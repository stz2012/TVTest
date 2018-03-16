// CasCard.h: CCasCard �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#pragma once


#include "BonBaseClass.h"
#include "CardReader.h"

// ECM�f�[�^�̍ŏ�/�ő�T�C�Y
#define MIN_ECM_DATA_SIZE 30
#define MAX_ECM_DATA_SIZE 256
// EMM�f�[�^�̍ő�T�C�Y
#define MAX_EMM_DATA_SIZE 263

class CCasCard : public CBonBaseClass
{
public:
	// �G���[�R�[�h
	enum {
		ERR_NOERROR			= 0x00000000UL,	// �G���[�Ȃ�
		ERR_INTERNALERROR	= 0x00000001UL,	// �����G���[
		ERR_NOTESTABLISHED	= 0x00000002UL,	// �R���e�L�X�g�m�����s
		ERR_NOCARDREADERS	= 0x00000003UL,	// �J�[�h���[�_���Ȃ�
		ERR_ALREADYOPEN		= 0x00000004UL,	// ���ɃI�[�v���ς�
		ERR_CARDOPENERROR	= 0x00000005UL,	// �J�[�h�I�[�v�����s
		ERR_CARDNOTOPEN		= 0x00000006UL,	// �J�[�h���I�[�v��
		ERR_TRANSMITERROR	= 0x00000007UL,	// �ʐM�G���[
		ERR_BADARGUMENT		= 0x00000008UL,	// �������s��
		ERR_ECMREFUSED		= 0x00000009UL,	// ECM��t����
		ERR_EMMERROR		= 0x0000000AUL,	// EMM�����G���[
		ERR_UNCONTRACTED	= 0x0000000BUL,	// ��_��
		ERR_PURCHASEAVAIL	= 0x0000000CUL,	// �w����
		ERR_PURCHASEREFUSED	= 0x0000000DUL,	// �w������
		ERR_SECURITY		= 0x0000000EUL,	// �Z�L�����e�B�G���[
		ERR_UNKNOWNCODE		= 0x0000000FUL,	// �s���ȃR�[�h
		ERR_NONOPERATIONAL	= 0x00000010UL	// ��^�p
	};

	enum {
		CARDTYPE_PREPAID	= 0x00,
		CARDTYPE_STANDARD	= 0x01,
		CARDTYPE_INVALID	= 0xFF
	};

	struct CasCardInfo
	{
		WORD CASystemID;				// CA_system_id
		BYTE CardID[6];					// Card ID
		BYTE CardType;					// Card type
		BYTE MessagePartitionLength;	// Message partition length
		BYTE SystemKey[32];				// Descrambling system key
		BYTE InitialCbc[8];				// Descrambler CBC initial value
		BYTE CardManufacturerID;		// Manufacturer identifier
		BYTE CardVersion;				// Version
		WORD CheckCode;					// Check code
	};

	CCasCard();
	~CCasCard();

	const DWORD GetCardReaderNum(void) const;
	LPCTSTR EnumCardReader(const DWORD dwIndex) const;

	const bool OpenCard(CCardReader::ReaderType ReaderType = CCardReader::READER_SCARD, LPCTSTR lpszReader = NULL);
	void CloseCard(void);
	const bool ReOpenCard();
	const bool IsCardOpen() const;
	CCardReader::ReaderType GetCardReaderType() const;
	LPCTSTR GetCardReaderName() const;

	const bool GetCasCardInfo(CasCardInfo *pInfo) const;
	const bool GetCASystemID(WORD *pID) const;
	const BYTE * GetCardID(void) const;
	const BYTE GetCardType(void) const;
	const BYTE GetMessagePartitionLength(void) const;
	const BYTE * GetInitialCbc(void) const;
	const BYTE * GetSystemKey(void) const;
	const char GetCardManufacturerID() const;
	const BYTE GetCardVersion() const;
	const int FormatCardID(LPTSTR pszText, int MaxLength) const;

	const BYTE * GetKsFromEcm(const BYTE *pEcmData, const DWORD dwEcmSize);
	const bool SendEmmSection(const BYTE *pEmmData, const DWORD dwEmmSize);
	const bool ConfirmContract(const BYTE *pVerificationData, const DWORD DataSize, const WORD Date);
	const bool SendCommand(const BYTE *pSendData, const DWORD SendSize, BYTE *pReceiveData, DWORD *pReceiveSize);

protected:
	const bool OpenAndInitialize(LPCTSTR pszReader);
	const bool InitialSetting(void);

	CCardReader *m_pCardReader;

	CasCardInfo m_CasCardInfo;

	struct TAG_ECMSTATUS
	{
		DWORD dwLastEcmSize;					// �Ō�ɖ₢���킹�̂�����ECM�T�C�Y
		BYTE LastEcmData[MAX_ECM_DATA_SIZE];	// �Ō�ɖ₢���킹�̂�����ECM�f�[�^
		BYTE KsData[16];						// Ks Odd + Even
		bool bSucceeded;						// ECM���󂯕t����ꂽ��
	} m_EcmStatus;
};
