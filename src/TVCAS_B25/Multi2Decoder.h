// Multi2Decoder.h: CMulti2Decoder �N���X�̃C���^�[�t�F�C�X
//
//////////////////////////////////////////////////////////////////////

#pragma once


#define MULTI2_USE_INTRINSIC	// �g�ݍ��݊֐��𗘗p
#define MULTI2_SIMD				// SIMD�Ή�

#ifdef MULTI2_SIMD
/*
	ICC���g�p���Ȃ��ꍇ�́AMULTI2_SIMD_ICC���`���Ȃ��悤�ɂ���
	(������20%���炢�x���Ȃ�)
*/
#define MULTI2_SSE2				// SSE2�Ή�
#define MULTI2_SSSE3			// SSSE3�Ή�
#define MULTI2_SIMD_ICC			// Intel C++ Compiler �𗘗p����

namespace Multi2DecoderSIMD {
	struct WORKKEY;
};
#endif	// MULTI2_SIMD


class CMulti2Decoder
{
public:
#ifdef MULTI2_SIMD
	enum InstructionType
	{
		INSTRUCTION_NORMAL,
		INSTRUCTION_SSE2,
		INSTRUCTION_SSSE3
	};
#endif

	CMulti2Decoder(
#ifdef MULTI2_SIMD
		InstructionType Instruction
#endif
		);
	~CMulti2Decoder(void);

	const bool Initialize(const BYTE *pSystemKey, const BYTE *pInitialCbc);
	const bool SetScrambleKey(const BYTE *pScrambleKey);
	const bool Decode(BYTE *pData, const DWORD dwSize, const BYTE byScrCtrl) const;

#ifdef MULTI2_SIMD
	static bool IsSSE2Available();
	static bool IsSSSE3Available();
#endif

	class SYSKEY	// System Key(Sk), Expanded Key(Wk) 256bit
	{
	public:
		inline void SetHexData(const BYTE *pHexData);
		inline void GetHexData(BYTE *pHexData) const;

		union {
#if !defined(MULTI2_USE_INTRINSIC) || !defined(_WIN64)
			struct {
				DWORD dwKey1, dwKey2, dwKey3, dwKey4, dwKey5, dwKey6, dwKey7, dwKey8;
			};
#else
			struct {
				DWORD dwKey2, dwKey1, dwKey4, dwKey3, dwKey6, dwKey5, dwKey8, dwKey7;
			};
			unsigned __int64 Data64[4];
#endif
			BYTE Data[32];
		};
	};

	class DATKEY	// Data Key(Dk) 64bit
	{
	public:
		inline void SetHexData(const BYTE *pHexData);
		inline void GetHexData(BYTE *pHexData) const;
		DATKEY &operator^=(const DATKEY &Operand) {
#ifndef _WIN64
			dwRight ^= Operand.dwRight;
			dwLeft ^= Operand.dwLeft;
#else
			Data64 ^= Operand.Data64;
#endif
			return *this;
		}

		union {
			struct {
				DWORD dwRight, dwLeft;
			};
			unsigned __int64 Data64;
			BYTE Data[8];
		};
	};

private:
	static void KeySchedule(SYSKEY &WorkKey, const SYSKEY &SysKey, DATKEY &DataKey);

	static inline void RoundFuncPi1(DATKEY &Block);
	static inline void RoundFuncPi2(DATKEY &Block, const DWORD dwK1);
	static inline void RoundFuncPi3(DATKEY &Block, const DWORD dwK2, const DWORD dwK3);
	static inline void RoundFuncPi4(DATKEY &Block, const DWORD dwK4);

	static inline const DWORD LeftRotate(const DWORD dwValue, const DWORD dwRotate);

	DATKEY m_InitialCbc;
	SYSKEY m_SystemKey;
	SYSKEY m_WorkKeyOdd, m_WorkKeyEven;

	bool m_bIsSysKeyValid;
	bool m_bIsWorkKeyValid;

#ifdef MULTI2_SIMD
	InstructionType m_Instruction;
	typedef void (*DecodeFunc)(BYTE * __restrict pData, const DWORD dwSize,
							   const SYSKEY * __restrict pWorkKey,
							   const Multi2DecoderSIMD::WORKKEY * __restrict pPackedWorkKey,
							   const DATKEY * __restrict pInitialCbc);
	DecodeFunc m_pDecodeFunc;
	Multi2DecoderSIMD::WORKKEY *m_pSSE2WorkKeyOdd, *m_pSSE2WorkKeyEven;
#endif
};
