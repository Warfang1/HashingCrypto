#pragma once
#include <sodium.h>

extern unsigned char publicKey[crypto_box_PUBLICKEYBYTES];
extern unsigned char privateKey[crypto_box_SECRETKEYBYTES];

void generateKeypair();
void encryptMAC(const char* mac, unsigned char* encrypted, unsigned char* nonce);
void decryptMAC(unsigned char* encrypted, unsigned char* nonce, unsigned char* decrypted, int macLen);
void hashPassword(const char* password, char* hashed);
bool verifyPassword(const char* password, const char* hashed);