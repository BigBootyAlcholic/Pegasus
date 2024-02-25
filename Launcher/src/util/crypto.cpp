#include "crypto.hpp"

//Cameron ahh code, why tmp

#define IS_ALIGNED_32(p) (0 == (3 & ((const char*)(p) - (const char*)0)))
#define ROTR64(Data, Bits) (((Data) >> Bits) | ((Data) << (64 - Bits)))
#define Sigma0(x) (ROTR64((x),28) ^ ROTR64((x),34) ^ ROTR64((x),39))
#define Sigma1(x) (ROTR64((x),14) ^ ROTR64((x),18) ^ ROTR64((x),41))
#define sigma0(x) (ROTR64((x),1) ^ ROTR64((x),8) ^ ((x)>>7))
#define sigma1(x) (ROTR64((x),19) ^ ROTR64((x),61) ^ ((x)>>6))
#define Ch(x,y,z) (((x) & (y)) ^ ((~(x)) & (z)))
#define Maj(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))

namespace Crypto {

	uint64_t g_sha512InitialState[XECRYPT_SHA512_DIGEST_SIZE / sizeof(uint64_t)] = {
		0x6A09E667F3BCC908ULL, 0xBB67AE8584CAA73BULL, 0x3C6EF372FE94F82BULL,
		0xA54FF53A5F1D36F1ULL, 0x510E527FADE682D1ULL, 0x9B05688C2B3E6C1FULL,
		0x1F83D9ABFB41BD6BULL, 0x5BE0CD19137E2179ULL
	};
	uint64_t g_sha512K[80] = {
		0x428A2F98D728AE22ULL, 0x7137449123EF65CDULL, 0xB5C0FBCFEC4D3B2FULL,
		0xE9B5DBA58189DBBCULL, 0x3956C25BF348B538ULL, 0x59F111F1B605D019ULL,
		0x923F82A4AF194F9BULL, 0xAB1C5ED5DA6D8118ULL, 0xD807AA98A3030242ULL,
		0x12835B0145706FBEULL, 0x243185BE4EE4B28CULL, 0x550C7DC3D5FFB4E2ULL,
		0x72BE5D74F27B896FULL, 0x80DEB1FE3B1696B1ULL, 0x9BDC06A725C71235ULL,
		0xC19BF174CF692694ULL, 0xE49B69C19EF14AD2ULL, 0xEFBE4786384F25E3ULL,
		0x0FC19DC68B8CD5B5ULL, 0x240CA1CC77AC9C65ULL, 0x2DE92C6F592B0275ULL,
		0x4A7484AA6EA6E483ULL, 0x5CB0A9DCBD41FBD4ULL, 0x76F988DA831153B5ULL,
		0x983E5152EE66DFABULL, 0xA831C66D2DB43210ULL, 0xB00327C898FB213FULL,
		0xBF597FC7BEEF0EE4ULL, 0xC6E00BF33DA88FC2ULL, 0xD5A79147930AA725ULL,
		0x06CA6351E003826FULL, 0x142929670A0E6E70ULL, 0x27B70A8546D22FFCULL,
		0x2E1B21385C26C926ULL, 0x4D2C6DFC5AC42AEDULL, 0x53380D139D95B3DFULL,
		0x650A73548BAF63DEULL, 0x766A0ABB3C77B2A8ULL, 0x81C2C92E47EDAEE6ULL,
		0x92722C851482353BULL, 0xA2BFE8A14CF10364ULL, 0xA81A664BBC423001ULL,
		0xC24B8B70D0F89791ULL, 0xC76C51A30654BE30ULL, 0xD192E819D6EF5218ULL,
		0xD69906245565A910ULL, 0xF40E35855771202AULL, 0x106AA07032BBD1B8ULL,
		0x19A4C116B8D2D0C8ULL, 0x1E376C085141AB53ULL, 0x2748774CDF8EEB99ULL,
		0x34B0BCB5E19B48A8ULL, 0x391C0CB3C5C95A63ULL, 0x4ED8AA4AE3418ACBULL,
		0x5B9CCA4F7763E373ULL, 0x682E6FF3D6B2B8A3ULL, 0x748F82EE5DEFB2FCULL,
		0x78A5636F43172F60ULL, 0x84C87814A1F0AB72ULL, 0x8CC702081A6439ECULL,
		0x90BEFFFA23631E28ULL, 0xA4506CEBDE82BDE9ULL, 0xBEF9A3F7B2C67915ULL,
		0xC67178F2E372532BULL, 0xCA273ECEEA26619CULL, 0xD186B8C721C0C207ULL,
		0xEADA7DD6CDE0EB1EULL, 0xF57D4F7FEE6ED178ULL, 0x06F067AA72176FBAULL,
		0x0A637DC5A2C898A6ULL, 0x113F9804BEF90DAEULL, 0x1B710B35131C471BULL,
		0x28DB77F523047D84ULL, 0x32CAAB7B40C72493ULL, 0x3C9EBE0A15C9BEBCULL,
		0x431D67C49C100D4CULL, 0x4CC5D4BECB3E42B6ULL, 0x597F299CFC657E2AULL,
		0x5FCB6FAB3AD6FAECULL, 0x6C44198C4A475817ULL
	};

	void Sha512Init(XECRYPT_SHA512_STATE* pShaState) {
		memset(pShaState, 0, sizeof(XECRYPT_SHA512_STATE));

		pShaState->count = 0;
		memcpy(&pShaState->state, g_sha512InitialState, sizeof(g_sha512InitialState));
	}

	void Sha512Transform(uint64_t* pDigest, uint64_t* pInp) {
		uint64_t* W = pInp;
		uint64_t X[XECRYPT_SHA512_BLOCK_SIZE / sizeof(uint64_t)];

		uint64_t A = pDigest[0];
		uint64_t B = pDigest[1];
		uint64_t C = pDigest[2];
		uint64_t D = pDigest[3];
		uint64_t E = pDigest[4];
		uint64_t F = pDigest[5];
		uint64_t G = pDigest[6];
		uint64_t H = pDigest[7];

		int i;

		for (i = 0; i < 16; ++i) {
			uint64_t Temp1 = X[i] = _byteswap_uint64(W[i]);
			uint64_t Temp2 = 0;

			Temp1 += H + Sigma1(E) + Ch(E, F, G) + g_sha512K[i];
			Temp2 = Sigma0(A) + Maj(A, B, C);

			H = G;
			G = F;
			F = E;
			E = D + Temp1;
			D = C;
			C = B;
			B = A;
			A = Temp1 + Temp2;
		}

		for (; i < 80; ++i) {
			uint64_t Temp1 = 0;
			uint64_t Temp2 = 0;

			uint64_t S0 = sigma0(X[(i + 1) & 15]);
			uint64_t S1 = sigma1(X[(i + 14) & 15]);

			Temp1 = X[i & 15] += S0 + S1 + X[(i + 9) & 15];
			Temp1 += H + Sigma1(E) + Ch(E, F, G) + g_sha512K[i];
			Temp2 = Sigma0(A) + Maj(A, B, C);

			H = G;
			G = F;
			F = E;
			E = D + Temp1;
			D = C;
			C = B;
			B = A;
			A = Temp1 + Temp2;
		}

		pDigest[0] += A;
		pDigest[1] += B;
		pDigest[2] += C;
		pDigest[3] += D;
		pDigest[4] += E;
		pDigest[5] += F;
		pDigest[6] += G;
		pDigest[7] += H;
	}

	void Sha512Update(XECRYPT_SHA512_STATE* pShaState, const BYTE* pbInp, DWORD cbInp) {
		DWORD Index = pShaState->count & 127;

		pShaState->count = pShaState->count + cbInp;

		if (Index) {
			if (Index + cbInp >= XECRYPT_SHA512_BLOCK_SIZE) {
				memcpy(&pShaState->buffer[Index], pbInp, Index - XECRYPT_SHA512_BLOCK_SIZE);

				Sha512Transform((uint64_t*)pShaState->state, (uint64_t*)pShaState->buffer);

				pbInp += XECRYPT_SHA512_BLOCK_SIZE;
				cbInp -= XECRYPT_SHA512_BLOCK_SIZE;
			}
		}

		if (cbInp >= XECRYPT_SHA512_BLOCK_SIZE) {
			DWORD Blocks = (Index + cbInp) / XECRYPT_SHA512_BLOCK_SIZE;

			if (IS_ALIGNED_32(pbInp)) {
				for (DWORD i = 0; i < Blocks; ++i) {
					Sha512Transform((uint64_t*)pShaState->state, (uint64_t*)pbInp);

					pbInp += XECRYPT_SHA512_BLOCK_SIZE;
					cbInp -= XECRYPT_SHA512_BLOCK_SIZE;
				}
			}
			else {
				for (DWORD i = 0; i < Blocks; ++i) {
					memcpy(pShaState->buffer, pbInp, XECRYPT_SHA512_BLOCK_SIZE);

					Sha512Transform((uint64_t*)pShaState->state, (uint64_t*)pShaState->buffer);

					pbInp += XECRYPT_SHA512_BLOCK_SIZE;
					cbInp -= XECRYPT_SHA512_BLOCK_SIZE;
				}
			}
		}

		if (cbInp) {
			memcpy(pShaState->buffer, pbInp, cbInp);
		}
	}

	void Sha512Final(XECRYPT_SHA512_STATE* pShaState, BYTE* pbOut, DWORD cbOut) {
		DWORD Count = pShaState->count;

		DWORD Index = Count & 127;

		memset(&pShaState->buffer[Index], 0, XECRYPT_SHA512_BLOCK_SIZE - Index);

		pShaState->buffer[Index] = 0x80;

		if (XECRYPT_SHA512_BLOCK_SIZE - Index < 17) {
			Sha512Transform((uint64_t*)pShaState->state, (uint64_t*)pShaState->buffer);

			memset(pShaState->buffer, 0, Index + 1);
		}

		Count = Count << 3;

		DWORD* New = (DWORD*)&pShaState->buffer[XECRYPT_SHA512_BLOCK_SIZE - sizeof(DWORD)];
		DWORD* Input = &Count;

		for (std::size_t i = 0; i < 1; ++i) {
			New[i] = _byteswap_ulong(Input[i]);
		}

		Sha512Transform((uint64_t*)pShaState->state, (uint64_t*)pShaState->buffer);

		if (cbOut != 0) {
			for (int i = 0; i < ARRAYSIZE(pShaState->state); ++i) {
				pShaState->state[i] = _byteswap_uint64(pShaState->state[i]);
			}

			if (cbOut < XECRYPT_SHA512_DIGEST_SIZE) {
				memcpy(pbOut, pShaState->state, cbOut);
			}
			else {
				memcpy(pbOut, pShaState->state, XECRYPT_SHA512_DIGEST_SIZE);
			}
		}
	}
}