#include <PubSubClient.h>

bool reconnect(PubSubClient& client);
void send_msg(PubSubClient& client, String topic, String mess);
void subscribe_topic(PubSubClient& client, char *topic);
