#include <iostream>
#include <sodium.h>
#include <cstring>

int main() {
    if (sodium_init() < 0) {
        std::cout << "libsodium failed!" << std::endl;
        return 1;
    }

    const char* password = "hello123";

    char hashed_password[crypto_pwhash_STRBYTES];

if (crypto_pwhash_str(hashed_password, password, strlen(password), crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
    std::cout << "Hashing failed!" << std::endl;
    return 1;
}

std::cout << "Hash: " << hashed_password << std::endl;

      if (crypto_pwhash_str_verify(hashed_password, password, strlen(password)) == 0) {
    std::cout << "Password matches!" << std::endl;
} else {
    std::cout << "Password does not match!" << std::endl;
}

    return 0;
}