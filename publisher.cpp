#include <windows.h> 
#include <iostream>
#include <cstring>
#include <MQTTClient.h>
#include <sodium.h>

#define BROKER_ADDRESS "tcp://localhost:1883"
#define CLIENT_ID      "publisher"
#define TOPIC          "password/hash"

int main() {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    // Create client
    MQTTClient_create(&client, BROKER_ADDRESS, CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);

    // Connect to broker
    if (MQTTClient_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS) {
        std::cout << "Failed to connect to broker!" << std::endl;
        return 1;
    }

    std::cout << "Connected to MQTT broker!" << std::endl;

   // Initialize libsodium
if (sodium_init() < 0) {
    std::cout << "libsodium failed!" << std::endl;
    return 1;
}

// Hash the password
const char* password = "hello123";
char hash[crypto_pwhash_STRBYTES];
if (crypto_pwhash_str(hash, password, strlen(password), crypto_pwhash_OPSLIMIT_INTERACTIVE, crypto_pwhash_MEMLIMIT_INTERACTIVE) != 0) {
    std::cout << "Hashing failed!" << std::endl;
    return 1;
}
std::cout << "Hash generated: " << hash << std::endl;

    // Create message
    MQTTClient_message message = MQTTClient_message_initializer;
    message.payload = hash;
    message.payloadlen = strlen(hash);
    message.qos = 1;
    message.retained = 0;

    // Send it
    MQTTClient_publishMessage(client, TOPIC, &message, NULL);
    MQTTClient_waitForCompletion(client, NULL, 1000);
    Sleep(1000);  // wait 1 second before disconnecting
    std::cout << "Hash sent!" << std::endl;

    MQTTClient_disconnect(client, 1000);
    MQTTClient_destroy(&client);
    return 0;
}