// CasCard.cpp: CCasCard �N���X�̃C���v�������e�[�V����
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CasCard.h"
#include "StdUtil.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// ���X�|���X��M�o�b�t�@�T�C�Y
#define RECEIVE_BUFFER_SIZE 1024

#define CARD_NOT_OPEN_ERROR_TEXT	TEXT("�J�[�h���[�_���J����Ă��܂���B")
#define BAD_ARGUMENT_ERROR_TEXT		TEXT("�������s���ł��B")
#define ECM_REFUSED_ERROR_TEXT		TEXT("ECM���󂯕t�����܂���B")


inline WORD GetReturnCode(const BYTE *pRecvData)
{
	return (WORD)((pRecvData[4] << 8) | pRecvData[5]);
}




CCasCard::CCasCard()
	: m_pCardReader(NULL)
{
	// ������ԏ�����
	::ZeroMemory(&m_CasCardInfo, sizeof(m_CasCardInfo));
	::ZeroMemory(&m_EcmStatus, sizeof(m_EcmStatus));
}


CCasCard::~CCasCard()
{
	CloseCard();
}


const DWORD CCasCard::GetCardReaderNum(void) const
{
	// �J�[�h���[�_�[����Ԃ�
	if (m_pCardReader)
		return m_pCardReader->NumReaders();
	return 0;
}


LPCTSTR CCasCard::EnumCardReader(const DWORD dwIndex) const
{
	if (m_pCardReader)
		return m_pCardReader->EnumReader(dwIndex);
	return NULL;
}


const bool CCasCard::OpenCard(CCardReader::ReaderType ReaderType, LPCTSTR lpszReader)
{
	// ��U�N���[�Y����
	CloseCard();

	m_pCardReader = CCardReader::CreateCardReader(ReaderType);
	if (m_pCardReader == NULL) {
		SetError(ERR_CARDOPENERROR, TEXT("�J�[�h���[�_�̃^�C�v�������ł��B"));
		return false;
	}

	bool bSuccess = false;

	if (lpszReader || m_pCardReader->NumReaders() <= 1) {
		// �w�肳�ꂽ���[�_�[���J��
		if (OpenAndInitialize(lpszReader))
			bSuccess = true;
	} else {
		// ���p�\�ȃ��[�_�[��T���ĊJ��
		LPCTSTR pszReaderName;

		for (int i = 0; (pszReaderName = m_pCardReader->EnumReader(i)) != NULL; i++) {
			if (OpenAndInitialize(pszReaderName)) {
				bSuccess = true;
				break;
			}
		}
	}

	if (!bSuccess) {
		delete m_pCardReader;
		m_pCardReader = NULL;
		return false;
	}

	ClearError();

	return true;
}


void CCasCard::CloseCard(void)
{
	// �J�[�h���N���[�Y����
	if (m_pCardReader) {
		m_pCardReader->Close();
		delete m_pCardReader;
		m_pCardReader = NULL;
	}
}


const bool CCasCard::ReOpenCard()
{
	if (m_pCardReader == NULL) {
		SetError(ERR_CARDNOTOPEN, CARD_NOT_OPEN_ERROR_TEXT);
		return false;
	}

	CCardReader::ReaderType Type = m_pCardReader->GetReaderType();
	LPTSTR pszReaderName = StdUtil::strdup(m_pCardReader->GetReaderName());
	bool bResult=OpenCard(Type, pszReaderName);
	delete [] pszReaderName;
	return bResult;
}


const bool CCasCard::IsCardOpen() const
{
	return m_pCardReader != NULL;
}


CCardReader::ReaderType CCasCard::GetCardReaderType() const
{
	if (m_pCardReader)
		return m_pCardReader->GetReaderType();
	return CCardReader::READER_NONE;
}


LPCTSTR CCasCard::GetCardReaderName() const
{
	if (m_pCardReader)
		return m_pCardReader->GetReaderName();
	return NULL;
}


const bool CCasCard::OpenAndInitialize(LPCTSTR pszReader)
{
	if (!m_pCardReader->Open(pszReader)) {
		SetError(m_pCardReader->GetLastErrorException());
		SetErrorCode(ERR_CARDOPENERROR);
		return false;
	}

	// �J�[�h������(���s�����烊�g���C���Ă݂�)
	if (!InitialSetting() && !InitialSetting()) {
		m_pCardReader->Close();
		return false;
	}

	return true;
}


const bool CCasCard::InitialSetting(void)
{
	// �uInitial Setting Conditions Command�v����������
	/*
	if (!m_pCardReader) {
		SetError(ERR_CARDNOTOPEN, CARD_NOT_OPEN_ERROR_TEXT);
		return false;
	}
	*/

	// �o�b�t�@����
	DWORD dwRecvSize;
	BYTE RecvData[RECEIVE_BUFFER_SIZE];

	// �����ݒ�����R�}���h���M
	static const BYTE InitSettingCmd[] = {0x90U, 0x30U, 0x00U, 0x00U, 0x00U};
	::ZeroMemory(RecvData, sizeof(RecvData));
	dwRecvSize = sizeof(RecvData);
	TRACE(TEXT("Send \"Initial Setting Conditions Command\"\n"));
	if (!m_pCardReader->Transmit(InitSettingCmd, sizeof(InitSettingCmd), RecvData, &dwRecvSize)) {
		SetError(ERR_TRANSMITERROR, m_pCardReader->GetLastErrorText());
		return false;
	}

	if (dwRecvSize < 57UL) {
		SetError(ERR_TRANSMITERROR, TEXT("��M�f�[�^�̃T�C�Y���s���ł��B"));
		return false;
	}

	// ���X�|���X���
	m_CasCardInfo.CASystemID = ((WORD)RecvData[6] << 8) | (WORD)RecvData[7];
	::CopyMemory(m_CasCardInfo.CardID, &RecvData[8], 6);		// +8	Card ID
	m_CasCardInfo.CardType = RecvData[14];
	m_CasCardInfo.MessagePartitionLength = RecvData[15];
	::CopyMemory(m_CasCardInfo.SystemKey, &RecvData[16], 32);	// +16	Descrambling system key
	::CopyMemory(m_CasCardInfo.InitialCbc, &RecvData[48], 8);	// +48	Descrambler CBC initial value

	if (::memcmp(m_CasCardInfo.CardID, "\0\0\0\0\0", 6) == 0) {
		SetError(ERR_TRANSMITERROR, TEXT("�J�[�hID���s���ł��B"));
		return false;
	}

	// �J�[�hID���擾�R�}���h���M
	static const BYTE CardIDInfoCmd[] = {0x90, 0x32, 0x00, 0x00, 0x00};
	::ZeroMemory(RecvData, sizeof(RecvData));
	dwRecvSize = sizeof(RecvData);
	TRACE(TEXT("Send \"Card ID Information Acquire Command\"\n"));
	if (!m_pCardReader->Transmit(CardIDInfoCmd, sizeof(CardIDInfoCmd), RecvData, &dwRecvSize)) {
		SetError(ERR_TRANSMITERROR, m_pCardReader->GetLastErrorText());
		return false;
	}

	if (dwRecvSize < 19) {
		SetError(ERR_TRANSMITERROR, TEXT("��M�f�[�^�̃T�C�Y���s���ł��B"));
		return false;
	}

	m_CasCardInfo.CardManufacturerID = RecvData[7];
	m_CasCardInfo.CardVersion = RecvData[8];
	m_CasCardInfo.CheckCode = ((WORD)RecvData[15] << 8) | (WORD)RecvData[16];

	if (::memcmp(&RecvData[9], "\0\0\0\0\0", 6) == 0) {
		SetError(ERR_TRANSMITERROR, TEXT("�J�[�hID���s���ł��B"));
		return false;
	}

	// ECM�X�e�[�^�X������
	::ZeroMemory(&m_EcmStatus, sizeof(m_EcmStatus));

	return true;
}


const bool CCasCard::GetCasCardInfo(CasCardInfo *pInfo) const
{
	if (!m_pCardReader) {
		//SetError(ERR_CARDNOTOPEN, CARD_NOT_OPEN_ERROR_TEXT);
		return false;
	}

	if (pInfo == NULL) {
		//SetError(ERR_BADARGUMENT, BAD_ARGUMENT_ERROR_TEXT);
		return false;
	}

	*pInfo = m_CasCardInfo;

	//ClearError();

	return true;
}


const bool CCasCard::GetCASystemID(WORD *pID) const
{
	if (!m_pCardReader) {
		//SetError(ERR_CARDNOTOPEN, CARD_NOT_OPEN_ERROR_TEXT);
		return false;
	}

	if (pID == NULL) {
		//SetError(ERR_BADARGUMENT, BAD_ARGUMENT_ERROR_TEXT);
		return false;
	}

	*pID = m_CasCardInfo.CASystemID;

	//ClearError();

	return true;
}


const BYTE * CCasCard::GetCardID(void) const
{
	// Card ID ��Ԃ�
	if (!m_pCardReader) {
		//SetError(ERR_CARDNOTOPEN, CARD_NOT_OPEN_ERROR_TEXT);
		return NULL;
	}

	//ClearError();

	return m_CasCardInfo.CardID;
}


const BYTE CCasCard::GetCardType(void) const
{
	if (!m_pCardReader) {
		//SetError(ERR_CARDNOTOPEN, CARD_NOT_OPEN_ERROR_TEXT);
		return CARDTYPE_INVALID;
	}

	//ClearError();

	return m_CasCardInfo.CardType;
}


const BYTE CCasCard::GetMessagePartitionLength(void) const
{
	return m_CasCardInfo.MessagePartitionLength;
}


const BYTE * CCasCard::GetInitialCbc(void) const
{
	// Descrambler CBC Initial Value ��Ԃ�
	if (!m_pCardReader) {
		//SetError(ERR_CARDNOTOPEN, CARD_NOT_OPEN_ERROR_TEXT);
		return NULL;
	}

	//ClearError();

	return m_CasCardInfo.InitialCbc;
}


const BYTE * CCasCard::GetSystemKey(void) const
{
	// Descrambling System Key ��Ԃ�
	if (!m_pCardReader) {
		//SetError(ERR_CARDNOTOPEN, CARD_NOT_OPEN_ERROR_TEXT);
		return NULL;
	}

	//ClearError();

	return m_CasCardInfo.SystemKey;
}


const char CCasCard::GetCardManufacturerID() const
{
	return m_CasCardInfo.CardManufacturerID;
}


const BYTE CCasCard::GetCardVersion() const
{
	return m_CasCardInfo.CardVersion;
}


const int CCasCard::FormatCardID(LPTSTR pszText, int MaxLength) const
{
	if (pszText == NULL || MaxLength <= 0)
		return 0;

	ULONGLONG ID;

	ID = ((((ULONGLONG)(m_CasCardInfo.CardID[0] & 0x1F) << 40) |
		   ((ULONGLONG)m_CasCardInfo.CardID[1] << 32) |
		   ((ULONGLONG)m_CasCardInfo.CardID[2] << 24) |
		   ((ULONGLONG)m_CasCardInfo.CardID[3] << 16) |
		   ((ULONGLONG)m_CasCardInfo.CardID[4] << 8) |
		    (ULONGLONG)m_CasCardInfo.CardID[5]) * 100000ULL) +
		 (ULONGLONG)m_CasCardInfo.CheckCode;
	return StdUtil::snprintf(pszText, MaxLength,
			TEXT("%d%03lu %04lu %04lu %04lu %04lu"),
			m_CasCardInfo.CardID[0] >> 5,
			(unsigned long)(ID / (10000ULL * 10000ULL * 10000ULL * 10000ULL)) % 10000,
			(unsigned long)(ID / (10000ULL * 10000ULL * 10000ULL)) % 10000,
			(unsigned long)(ID / (10000ULL * 10000ULL) % 10000ULL),
			(unsigned long)(ID / 10000ULL % 10000ULL),
			(unsigned long)(ID % 10000ULL));
}


const BYTE * CCasCard::GetKsFromEcm(const BYTE *pEcmData, const DWORD dwEcmSize)
{
	// �uECM Receive Command�v����������
	if (!m_pCardReader) {
		SetError(ERR_CARDNOTOPEN, CARD_NOT_OPEN_ERROR_TEXT);
		return NULL;
	}

	// ECM�T�C�Y���`�F�b�N
	if (!pEcmData || (dwEcmSize < MIN_ECM_DATA_SIZE) || (dwEcmSize > MAX_ECM_DATA_SIZE)) {
		SetError(ERR_BADARGUMENT, BAD_ARGUMENT_ERROR_TEXT);
		return NULL;
	}

	// �L���b�V�����`�F�b�N����
	if (m_EcmStatus.dwLastEcmSize == dwEcmSize
			&& ::memcmp(m_EcmStatus.LastEcmData, pEcmData, dwEcmSize) == 0) {
		// ECM������̏ꍇ�̓L���b�V���ς�Ks��Ԃ�
		if (m_EcmStatus.bSucceeded) {
			ClearError();
			return m_EcmStatus.KsData;
		} else {
			SetError(ERR_ECMREFUSED, ECM_REFUSED_ERROR_TEXT);
			return NULL;
		}
	}

	// �o�b�t�@����
	static const BYTE EcmReceiveCmd[] = {0x90, 0x34, 0x00, 0x00};
	BYTE SendData[MAX_ECM_DATA_SIZE + 6];
	BYTE RecvData[RECEIVE_BUFFER_SIZE];
	::ZeroMemory(RecvData, sizeof(RecvData));

	// �R�}���h�\�z
	::CopyMemory(SendData, EcmReceiveCmd, sizeof(EcmReceiveCmd));				// CLA, INS, P1, P2
	SendData[sizeof(EcmReceiveCmd)] = (BYTE)dwEcmSize;							// COMMAND DATA LENGTH
	::CopyMemory(&SendData[sizeof(EcmReceiveCmd) + 1], pEcmData, dwEcmSize);	// ECM
	SendData[sizeof(EcmReceiveCmd) + dwEcmSize + 1] = 0x00U;					// RESPONSE DATA LENGTH

	// �R�}���h���M
	DWORD dwRecvSize = sizeof(RecvData);
	if (!m_pCardReader->Transmit(SendData, sizeof(EcmReceiveCmd) + dwEcmSize + 2UL, RecvData, &dwRecvSize)){
		::ZeroMemory(&m_EcmStatus, sizeof(m_EcmStatus));
		SetError(ERR_TRANSMITERROR, m_pCardReader->GetLastErrorText());
		return NULL;
	}

	// �T�C�Y�`�F�b�N
	if (dwRecvSize != 25UL) {
		::ZeroMemory(&m_EcmStatus, sizeof(m_EcmStatus));
		SetError(ERR_TRANSMITERROR, TEXT("ECM�̃��X�|���X�T�C�Y���s���ł��B"));
		return NULL;
	}

	// ECM�f�[�^��ۑ�����
	m_EcmStatus.dwLastEcmSize = dwEcmSize;
	::CopyMemory(m_EcmStatus.LastEcmData, pEcmData, dwEcmSize);

	// ���X�|���X���
	::CopyMemory(m_EcmStatus.KsData, &RecvData[6], sizeof(m_EcmStatus.KsData));

	// ���^�[���R�[�h���
	switch (GetReturnCode(RecvData)) {
	// Purchased: Viewing
	case 0x0200U :	// Payment-deferred PPV
	case 0x0400U :	// Prepaid PPV
	case 0x0800U :	// Tier

	case 0x4480U :	// Payment-deferred PPV
	case 0x4280U :	// Prepaid PPV
		ClearError();
		m_EcmStatus.bSucceeded = true;
		return m_EcmStatus.KsData;
	}
	// ��L�ȊO(�����s��)

	m_EcmStatus.bSucceeded = false;
	SetError(ERR_ECMREFUSED, ECM_REFUSED_ERROR_TEXT);

	return NULL;
}


const bool CCasCard::SendEmmSection(const BYTE *pEmmData, const DWORD dwEmmSize)
{
	// �uEMM Receive Command�v����������
	if (!m_pCardReader) {
		SetError(ERR_CARDNOTOPEN, CARD_NOT_OPEN_ERROR_TEXT);
		return false;
	}

	if (pEmmData == NULL || dwEmmSize < 17UL || dwEmmSize > MAX_EMM_DATA_SIZE) {
		SetError(ERR_BADARGUMENT, BAD_ARGUMENT_ERROR_TEXT);
		return false;
	}

	TRACE(TEXT("Send \"EMM Receive Command\"\n"));

	static const BYTE EmmReceiveCmd[] = {0x90, 0x36, 0x00, 0x00};
	BYTE SendData[MAX_EMM_DATA_SIZE + 6], RecvData[RECEIVE_BUFFER_SIZE];

	::CopyMemory(SendData, EmmReceiveCmd, sizeof(EmmReceiveCmd));
	SendData[sizeof(EmmReceiveCmd)] = (BYTE)dwEmmSize;
	::CopyMemory(&SendData[sizeof(EmmReceiveCmd) + 1], pEmmData, dwEmmSize);
	SendData[sizeof(EmmReceiveCmd) + 1 + dwEmmSize] = 0x00;

	::ZeroMemory(RecvData, sizeof(RecvData));
	DWORD RecvSize = sizeof(RecvData);
	if (!m_pCardReader->Transmit(SendData, sizeof(EmmReceiveCmd) + dwEmmSize + 2UL, RecvData, &RecvSize)) {
		SetError(ERR_TRANSMITERROR, m_pCardReader->GetLastErrorText());
		return false;
	}

	if (RecvSize != 8UL) {
		SetError(ERR_TRANSMITERROR, TEXT("EMM�̃��X�|���X�T�C�Y���s���ł��B"));
		return false;
	}

	const WORD ReturnCode = GetReturnCode(RecvData);
	TRACE(TEXT(" -> Return Code %04x\n"), ReturnCode);
	switch (ReturnCode) {
	case 0x2100U :	// ����I��
		ClearError();
		return true;

	case 0xA102U :	// ��^�p(�^�p�O�v���g�R���ԍ�)
		SetError(ERR_EMMERROR, TEXT("�v���g�R���ԍ����^�p�O�ł��B"));
		break;

	case 0xA107U :	// �Z�L�����e�B�G���[(EMM������G���[)
		SetError(ERR_EMMERROR, TEXT("�Z�L�����e�B�G���[�ł��B"));
		break;

	default:
		SetError(ERR_EMMERROR, TEXT("EMM���󂯕t�����܂���B"));
		break;
	}

	return false;
}


const bool CCasCard::ConfirmContract(const BYTE *pVerificationData, const DWORD DataSize, const WORD Date)
{
	// �uContract Confirmation Command�v����������
	if (!m_pCardReader) {
		SetError(ERR_CARDNOTOPEN, CARD_NOT_OPEN_ERROR_TEXT);
		return false;
	}

	if (pVerificationData == NULL || DataSize < 1 || DataSize > 253) {
		SetError(ERR_BADARGUMENT, BAD_ARGUMENT_ERROR_TEXT);
		return false;
	}

	TRACE(TEXT("Send \"Contract Confirmation Command\"\n"));

	static const BYTE ContractConfirmCmd[] = {0x90, 0x3C, 0x00, 0x00};
	BYTE SendData[255 + 6], RecvData[RECEIVE_BUFFER_SIZE];

	::CopyMemory(SendData, ContractConfirmCmd, sizeof(ContractConfirmCmd));
	SendData[sizeof(ContractConfirmCmd)] = (BYTE)(DataSize + 2);
	SendData[sizeof(ContractConfirmCmd) + 1] = Date >> 8;
	SendData[sizeof(ContractConfirmCmd) + 2] = Date & 0xFF;
	::CopyMemory(&SendData[sizeof(ContractConfirmCmd) + 3], pVerificationData, DataSize);
	SendData[sizeof(ContractConfirmCmd) + 3 + DataSize] = 0x00;

	::ZeroMemory(RecvData, sizeof(RecvData));
	DWORD RecvSize = sizeof(RecvData);
	if (!m_pCardReader->Transmit(SendData, sizeof(ContractConfirmCmd) + DataSize + 4UL, RecvData, &RecvSize)) {
		SetError(ERR_TRANSMITERROR, m_pCardReader->GetLastErrorText());
		return false;
	}

	if (RecvSize != 20UL) {
		SetError(ERR_TRANSMITERROR, TEXT("�_��m�F�̃��X�|���X�T�C�Y���s���ł��B"));
		return false;
	}

	const WORD ReturnCode = GetReturnCode(RecvData);
	TRACE(TEXT(" -> Return Code %04x\n"), ReturnCode);
	switch (ReturnCode) {
	// �w����
	case 0x0800:	// �e�B�A
	case 0x0400:	// �O����PPV
	case 0x0200:	// �㕥��PPV
		ClearError();
		return true;

	case 0x8901:	// ��_��:�_��O(�e�B�A)
	case 0x8501:	// ��_��:�_��O(�O����PPV)
	case 0x8301:	// ��_��:�_��O(�㕥��PPV)
	case 0x8902:	// ��_��:�����؂�(�e�B�A)
	case 0x8502:	// ��_��:�����؂�(�O����PPV)
	case 0x8302:	// ��_��:�����؂�(�㕥��PPV)
	case 0x8903:	// ��_��:��������(�e�B�A)
	case 0x8503:	// ��_��:��������(�O����PPV)
	case 0x8303:	// ��_��:��������(�㕥��PPV)
	case 0xA103:	// ��_��(Kw�Ȃ�)
		SetError(ERR_UNCONTRACTED, TEXT("�_�񂳂�Ă��܂���B"));
		break;

	case 0x8500:	// �w����(�O����PPV)
	case 0x8300:	// �w����(�㕥��PPV)
		SetError(ERR_PURCHASEAVAIL, TEXT("�w������Ă��܂���B"));
		break;

	case 0x8109:	// �w������(�����������������t)
	case 0x850F:	// �w������(�O�����c���s��)
		SetError(ERR_PURCHASEREFUSED, TEXT("�w���ł��܂���B"));
		break;

	case 0xA102:	// ��^�p�J�[�h(�^�p�O�v���g�R���ԍ�)
		SetError(ERR_NONOPERATIONAL, TEXT("�v���g�R���ԍ����^�p�O�ł��B"));
		break;

	case 0xA104:	// �Z�L�����e�B�G���[(�_��m�F��������G���[)
		SetError(ERR_SECURITY, TEXT("�Z�L�����e�B�G���[�ł��B"));
		break;

	default:
		SetError(ERR_UNKNOWNCODE, TEXT("�s���ȃ��^�[���R�[�h�ł��B"));
		break;
	}

	return false;
}


const bool CCasCard::SendCommand(const BYTE *pSendData, const DWORD SendSize, BYTE *pReceiveData, DWORD *pReceiveSize)
{
	if (!m_pCardReader) {
		SetError(ERR_CARDNOTOPEN, CARD_NOT_OPEN_ERROR_TEXT);
		return false;
	}

	if (pSendData == NULL || SendSize == 0
			|| pReceiveData == NULL || pReceiveSize == NULL) {
		SetError(ERR_BADARGUMENT, BAD_ARGUMENT_ERROR_TEXT);
		return false;
	}

	if (!m_pCardReader->Transmit(pSendData, SendSize, pReceiveData, pReceiveSize)) {
		SetError(ERR_TRANSMITERROR, m_pCardReader->GetLastErrorText());
		return false;
	}

	ClearError();
	return true;
}
