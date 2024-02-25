#pragma once
#include "stdafx.hpp"

namespace Crypto {

#define XECRYPT_SHA512_DIGEST_SIZE 64
#define XECRYPT_SHA512_BLOCK_SIZE 128

	typedef struct _XECRYPT_SHA512_STATE {
		DWORD count;
		uint64_t state[8];
		BYTE buffer[128];
	} XECRYPT_SHA512_STATE, * PXECRYPT_SHA512_STATE;

	void Sha512Init(XECRYPT_SHA512_STATE* pShaState);
	void Sha512Update(XECRYPT_SHA512_STATE* pShaState, const BYTE* pbInp, DWORD cbInp);
	void Sha512Final(XECRYPT_SHA512_STATE* pShaState, BYTE* pbOut, DWORD cbOut);

}