/*
 *  Copyright (c) 2018-2019, SLikeSoft UG (haftungsbeschränkt)
 *
 *  This source code is  licensed under the MIT-style license found in the license.txt
 *  file in the root directory of this source tree.
 */
#pragma once

#include "ifileencrypter.h"    // used for Crypto::IFileEncrypter
#include "securestring.h"      // used for Crypto::CSecureString
#include <openssl/evp.h>       // used for EVP_xxxx

namespace SLNet::Experimental::Crypto
{
class CFileEncrypter : public IFileEncrypter
{
	// member variables
	EVP_PKEY *m_privateKey = nullptr;
	EVP_PKEY *m_publicKey = nullptr;
	unsigned char m_sigBuffer[1024];
	char m_sigBufferBase64[1369]; // 1369 = 1368 (size of base64-encoded 1k signature which is 1024 / 3 * 4 (representing 1023 bytes) + 4 bytes for the last byte) + 1 byte for trailing \0-terminator

	// constructor
public:
	// #high - drop the default ctor again (provide load from file instead incl. routing through customized file open handlers)
	CFileEncrypter();
	CFileEncrypter(const char *publicKey, size_t publicKeyLength);
	CFileEncrypter(const char *publicKey, size_t publicKeyLength, const char *privateKey, size_t privateKeyLength, CSecureString &password);
	~CFileEncrypter();

	// signing methods
public:
	const unsigned char *SignData(const unsigned char *data, const size_t dataLength) override;
	const char *SignDataBase64(const unsigned char *data, const size_t dataLength) override;
	// #med reconsider/review interface here (char / unsigned char)
	bool VerifyData(const unsigned char *data, const size_t dataLength, const unsigned char *signature, const size_t signatureLength) override;
	bool VerifyDataBase64(const unsigned char *data, const size_t dataLength, const char *signature, const size_t signatureLength) override;

	// internal helpers
private:
	const char *SetPrivateKey(const char *privateKey, size_t privateKeyLength, CSecureString &password);
	const char *SetPublicKey(const char *publicKey, size_t publicKeyLength);
};
}
