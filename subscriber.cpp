#include <sodium.h>
#include <iostream>
#include <cstring>
#include <MQTTClient.h>

#define BROKER_ADDRESS "tcp://localhost:1883"
#define CLIENT_ID      "subscriber"
#define TOPIC          "password/hash"

int messageArrived(void* context, char* topicName, int topicLen, MQTTClient_message* message) {
    char* hash = (char*)message->payload;
    std::cout << "Received hash: " << hash << std::endl;

    const char* password = "hello123";
if (crypto_pwhash_str_verify(hash, password, strlen(password)) == 0) {
    std::cout << "Hash is valid! Password matches!" << std::endl;
} else {
    std::cout << "Hash is invalid! Password does not match!" << std::endl;
}

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);
    return 1;
}

int main() {
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

    MQTTClient_create(&client, BROKER_ADDRESS, CLIENT_ID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
    MQTTClient_setCallbacks(client, NULL, NULL, messageArrived, NULL);

    if (MQTTClient_connect(client, &conn_opts) != MQTTCLIENT_SUCCESS) {
        std::cout << "Failed to connect to broker!" << std::endl;
        return 1;
    }

    std::cout << "Connected to MQTT broker!" << std::endl;

    MQTTClient_subscribe(client, TOPIC, 1);
    std::cout << "Waiting for messages..." << std::endl;

    while(true) {}

    MQTTClient_disconnect(client, 1000);
    MQTTClient_destroy(&client);
    return 0;
}