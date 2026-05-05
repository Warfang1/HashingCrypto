#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <cstring>
#include <sodium.h>
#include <MQTTClient.h>
#include "crypto_utils.hpp"

#define BROKER_ADDRESS "tcp://localhost:1883"
#define CLIENT_ID      "kx_client"
#define TOPIC_SERVER_PK "keyexchange/server_pk"
#define TOPIC_CLIENT_PK "keyexchange/client_pk"

// Global variables
unsigned char client_pk[crypto_kx_PUBLICKEYBYTES];
unsigned char client_sk[crypto_kx_SECRETKEYBYTES];
unsigned char server_pk[crypto_kx_PUBLICKEYBYTES];
unsigned char client_rx[crypto_kx_SESSIONKEYBYTES];
unsigned char client_tx[crypto_kx_SESSIONKEYBYTES];
MQTTClient mqttclient;

int messageArrived(void* context, char* topicName, int topicLen, MQTTClient_message* message) {
    std::cout << "Received server public key!" << std::endl;
    memcpy(server_pk, message->payload, crypto_kx_PUBLICKEYBYTES);

    // Compute session keys
    if (crypto_kx_client_session_keys(client_rx, client_tx, client_pk, client_sk, server_pk) != 0) {
        std::cout << "Key exchange failed!" << std::endl;
    } else {
        std::cout << "Session keys computed successfully!" << std::endl;
        char pk_hash[crypto_pwhash_STRBYTES];
        hashPassword((const char*)server_pk, pk_hash);
        std::cout << "Server public key hash: " << pk_hash << std::endl;
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

    // Generate client keypair
    crypto_kx_keypair(client_pk, client_sk);
    std::cout << "Client keypair generated!" << std::endl;

    // Connect to MQTT broker
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_create(&mqttclient, BROKER_ADDRESS, CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(mqttclient, NULL, NULL, messageArrived, NULL);

    if (MQTTClient_connect(mqttclient, &conn_opts) != MQTTCLIENT_SUCCESS) {
        std::cout << "Failed to connect to broker!" << std::endl;
        return 1;
    }
    std::cout << "Connected to MQTT broker!" << std::endl;

    // Subscribe to server's public key topic
    MQTTClient_subscribe(mqttclient, TOPIC_SERVER_PK, 1);
    std::cout << "Waiting for server public key..." << std::endl;

    // Publish client's public key
    MQTTClient_message msg = MQTTClient_message_initializer;
    msg.payload = client_pk;
    msg.payloadlen = crypto_kx_PUBLICKEYBYTES;
    msg.qos = 1;
    msg.retained = 0;
    MQTTClient_publishMessage(mqttclient, TOPIC_CLIENT_PK, &msg, NULL);
    std::cout << "Client public key published!" << std::endl;

    // Wait for server
    while(true) {}

    MQTTClient_disconnect(mqttclient, 1000);
    MQTTClient_destroy(&mqttclient);
    return 0;
}