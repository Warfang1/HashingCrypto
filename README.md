# HashingCrypto

Et C++ projekt der demonstrerer kryptografi og sikker kommunikation.

## Hvad projektet gør

- **Hashing af passwords** med libsodium (Argon2id algoritmen)
- **Verificering af passwords** mod en gemt hash
- **Klient/server kommunikation** via sockets (Winsock)
- **MQTT kommunikation** via Mosquitto broker
- **Asymmetrisk kryptering** af MAC-adresser med libsodium
- **Key exchange** mellem klient og server via MQTT med verificering af public keys

## Filer

- `server.cpp` – Modtager password fra klient, hasher og gemmer det
- `client.cpp` – Sender password til server og modtager svar
- `publisher.cpp` – Sender hash over MQTT
- `subscriber.cpp` – Modtager og verificerer hash over MQTT
- `asymmetricfunc.cpp` – Krypterer og dekrypterer MAC-adresse
- `kx_server.cpp` – Key exchange server, udveksler public keys og beregner session keys via MQTT
- `kx_client.cpp` – Key exchange klient, udveksler public keys og beregner session keys via MQTT
- `crypto_utils.hpp` – Header fil med funktions-deklarationer
- `crypto_utils.cpp` – Implementering af kryptografi-funktioner

## Teknologier

- C++
- libsodium
- Winsock
- Paho MQTT
- Mosquitto

## Sådan kører du projektet

### Password hashing (server/client)
1. Kør `server.exe` i terminal 1
2. Kør `client.exe` i terminal 2 og indtast et password

### MQTT hashing (publisher/subscriber)
1. Kør `subscriber.exe` i terminal 1
2. Kør `publisher.exe` i terminal 2

### Key exchange
1. Kør `kx_server.exe` i terminal 1
2. Kør `kx_client.exe` i terminal 2 inden for 10 sekunder