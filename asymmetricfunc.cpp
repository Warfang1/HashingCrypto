#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <sodium.h>
#include <cstring>
#include "crypto_utils.hpp"

int main() {
    if (sodium_init() < 0) {
        std::cout << "libsodium failed!" << std::endl;
        return 1;
    }

    const char* mac = "00:1A:2B:3C:4D:5E";
    int macLen = strlen(mac);

    generateKeypair();
    std::cout << "Keypair generated!" << std::endl;

    unsigned char encrypted[crypto_box_MACBYTES + macLen];
    unsigned char nonce[crypto_box_NONCEBYTES];
    encryptMAC(mac, encrypted, nonce);
    std::cout << "MAC address encrypted!" << std::endl;

    unsigned char decrypted[macLen + 1];
    decryptMAC(encrypted, nonce, decrypted, macLen);
    std::cout << "Decrypted MAC address: " << decrypted << std::endl;

    char hashed[crypto_pwhash_STRBYTES];
    std::cout << "Starting hashing..." << std::endl;
    hashPassword("hello123", hashed);
    std::cout << "Password hashed!" << std::endl;

    if (verifyPassword("hello123", hashed)) {
        std::cout << "Password verified!" << std::endl;
    } else {
        std::cout << "Password verification failed!" << std::endl;
    }

    return 0;
}