#include "messageTCP.h"


messageTCP::messageTCP()
{
  _client = WiFiClient();
}

messageTCP::~messageTCP()
{
  _client.stop();
}

void messageTCP::send(IPAddress ip, char* data)
{
  _client.connect(ip, 7084);
  _client.print(data);
  _client.stop();
}

void messageTCP::send(IPAddress ip, const uint8_t* data, size_t size)
{
  _client.connect(ip, 7084);
  _client.write(data, size);
  _client.stop();
}

uint8_t *messageTCP::receive(WiFiServer *server)
{
  WiFiClient client = server->available(); // Get the client
  while (client.connected() && !client.available())
  {
    yield();
  }
  if (!client.connected())
  {
    return NULL;
  }

  uint8_t dataSize = client.read();      // Read the size of the data
  uint8_t *data = new uint8_t[dataSize]; // Create a new array with the size of the data
  data[0] = dataSize;                    // Set the first byte of the array to the size of the data

  for (int i = 1; i < dataSize; i++) // Read the data
  {
    while (!client.available() && client.connected()) // Wait for data to be available
    {
      yield();
    }
    if (!client.connected())
    {
      return NULL;
    }
    data[i] = client.read(); // Read the data
  }

  client.stop(); // Stop the client
  return data;
}

