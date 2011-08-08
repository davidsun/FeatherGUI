#ifndef CRYPT_H
#define CRYPT_H

namespace Crypt{
	char *binaryToHex(const unsigned char *, int);
	char intToHex(int);
	int char64ToInt(char);
	int hexToInt(char);
	unsigned char *fetionDigestEncrypt(const char *);
	unsigned char *fetionEncrypt(const char *, const char *, const char *, const char *);
	unsigned char *hexToBinary(const char *, int);
	unsigned char *sha1Encrypt(const unsigned char *, int);
	unsigned char *string64ToBinary(const char *, int, int &);
};

#endif
