#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <cstring>
#include <sodium.h>

int main() {
    // Initialize libsodium
    if (sodium_init() < 0) {
        std::cout << "libsodium failed!" << std::endl;
        return 1;
    }

    // Generate keypair
    unsigned char publicKey[crypto_box_PUBLICKEYBYTES];
    unsigned char privateKey[crypto_box_SECRETKEYBYTES];
    crypto_box_keypair(publicKey, privateKey);

    std::cout << "Keypair generated!" << std::endl;

    // The MAC address to encrypt
    const char* mac = "00:1A:2B:3C:4D:5E";

    // We need a nonce (a random number used once)
    unsigned char nonce[crypto_box_NONCEBYTES];
    randombytes_buf(nonce, sizeof(nonce));

    // Buffer for the encrypted result
    unsigned char encrypted[crypto_box_MACBYTES + strlen(mac)];

    // Encrypt!
    crypto_box_easy(encrypted, (unsigned char*)mac, strlen(mac), nonce, publicKey, privateKey);

    std::cout << "MAC address encrypted!" << std::endl;

    // Buffer for decrypted result
    unsigned char decrypted[strlen(mac)];

    // Decrypt!
    if (crypto_box_open_easy(decrypted, encrypted, sizeof(encrypted), nonce, publicKey, privateKey) != 0) {
        std::cout << "Decryption failed!" << std::endl;
        return 1;
    }

    decrypted[strlen(mac)] = '\0';
    
    std::cout << "Decrypted MAC address: " << decrypted << std::endl;

    return 0;
}