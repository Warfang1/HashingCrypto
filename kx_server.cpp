#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <winsock2.h>
#include <iostream>
#include <cstring>
#include <sodium.h>
#include <MQTTClient.h>
#include "crypto_utils.hpp"

#define BROKER_ADDRESS "tcp://localhost:1883"
#define CLIENT_ID      "kx_server"
#define TOPIC_SERVER_PK "keyexchange/server_pk"
#define TOPIC_CLIENT_PK "keyexchange/client_pk"

// Global variables
unsigned char server_pk[crypto_kx_PUBLICKEYBYTES];
unsigned char server_sk[crypto_kx_SECRETKEYBYTES];
unsigned char client_pk[crypto_kx_PUBLICKEYBYTES];
unsigned char server_rx[crypto_kx_SESSIONKEYBYTES];
unsigned char server_tx[crypto_kx_SESSIONKEYBYTES];
MQTTClient client;

int messageArrived(void* context, char* topicName, int topicLen, MQTTClient_message* message) {
    std::cout << "Received client public key!" << std::endl;
    memcpy(client_pk, message->payload, crypto_kx_PUBLICKEYBYTES);

    // Compute session keys
    if (crypto_kx_server_session_keys(server_rx, server_tx, server_pk, server_sk, client_pk) != 0) {
        std::cout << "Key exchange failed!" << std::endl;
   } else {
        std::cout << "Session keys computed successfully!" << std::endl;
        char pk_hash[crypto_pwhash_STRBYTES];
        hashPassword((const char*)client_pk, pk_hash);
        std::cout << "Client public key hash: " << pk_hash << std::endl;
    }

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

int main() {
    if (sodium_init() < 0) {
        std::cout << "libsodium failed!" << std::endl;
        return 1;
    }

    // Generate server keypair
    crypto_kx_keypair(server_pk, server_sk);
    std::cout << "Server keypair generated!" << std::endl;

    // Connect to MQTT broker
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_create(&client, BROKER_ADDRESS, CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(client, NULL, NULL, messageArrived, NULL);

    if (MQTTClient_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS) {
        std::cout << "Failed to connect to broker!" << std::endl;
        return 1;
    }
    std::cout << "Connected to MQTT broker!" << std::endl;

    // Subscribe to client's public key topic
    MQTTClient_subscribe(client, TOPIC_CLIENT_PK, 1);
    std::cout << "Waiting for client public key..." << std::endl;

    // Wait for client to subscribe
    Sleep(10000);

    // Publish server's public key
    MQTTClient_message msg = MQTTClient_message_initializer;
    msg.payload = server_pk;
    msg.payloadlen = crypto_kx_PUBLICKEYBYTES;
    msg.qos = 1;
    msg.retained = 1;
    MQTTClient_publishMessage(client, TOPIC_SERVER_PK, &msg, NULL);
    std::cout << "Server public key published!" << std::endl;

    // Wait for client
    while(true) {}

    MQTTClient_disconnect(client, 1000);
    MQTTClient_destroy(&client);
    return 0;
}