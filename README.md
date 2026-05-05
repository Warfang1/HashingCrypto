# HashingCrypto

Et C++ projekt der demonstrerer kryptografi og sikker kommunikation.

## Hvad projektet gør

- **Hashing af passwords** med libsodium (Argon2id algoritmen)
- **Verificering af passwords** mod en gemt hash
- **Klient/server kommunikation** via sockets (Winsock)
- **MQTT kommunikation** via Mosquitto broker
- **Asymmetrisk kryptering** af MAC-adresser med libsodium

## Filer

- `server.cpp` – Modtager password fra klient, hasher og gemmer det
- `client.cpp` – Sender password til server og modtager svar
- `publisher.cpp` – Sender hash over MQTT
- `subscriber.cpp` – Modtager og verificerer hash over MQTT
- `asymmetricfunc.cpp` – Krypterer og dekrypterer MAC-adresse
- `crypto_utils.hpp` – Header fil med funktions-deklarationer
- `crypto_utils.cpp` – Implementering af kryptografi-funktioner

## Teknologier

- C++
- libsodium
- Winsock
- Paho MQTT
- Mosquitto