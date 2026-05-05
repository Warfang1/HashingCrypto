#include "crypto_utils.hpp"
#include <cstring>
#include <sodium.h>

unsigned char publicKey[crypto_box_PUBLICKEYBYTES];
unsigned char privateKey[crypto_box_SECRETKEYBYTES];

void generateKeypair() {
    crypto_box_keypair(publicKey, privateKey);
}

void encryptMAC(const char* mac, unsigned char* encrypted, unsigned char* nonce) {
    randombytes_buf(nonce, crypto_box_NONCEBYTES);
    crypto_box_easy(encrypted, (unsigned char*)mac, strlen(mac), nonce, publicKey, privateKey);
}

void decryptMAC(unsigned char* encrypted, unsigned char* nonce, unsigned char* decrypted, int macLen) {
    crypto_box_open_easy(decrypted, encrypted, crypto_box_MACBYTES + macLen, nonce, publicKey, privateKey);
    decrypted[macLen] = '\0';
}

void hashPassword(const char* password, char* hashed) {
    crypto_pwhash_str(hashed, password, strlen(password), crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE);
}

bool verifyPassword(const char* password, const char* hashed) {
    return crypto_pwhash_str_verify(hashed, password, strlen(password)) == 0;
}