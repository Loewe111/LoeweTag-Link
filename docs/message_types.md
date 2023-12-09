# Message Types

## Serial: PC -> ESP

Getting all connected Devices

```JSON
{
  "type": "get_devices"
}
```

Sending a message to a Device

```JSON
{
  "type": "send",
  "ip": "192.168.4.255", // 255.255.255.255 for broadcast
  "content": "JSON" // the content to send in JSON
}
```

### Types of Messages to a Pistol

Setting the Color

```JSON
{
  "type": "color",
  "color": [255, 255, 255] // RGB
}
```

Updating the Game variables

```JSON
{
  "type": "vars",
  "HP": 100, //the health of the player
  "MHP": 100, //the max health of the player
  "SP": 100, //the shield of the player (only used to display)
  "MSP": 100, //the max shield of the player (only used to display)
  "ATK": 10, //the attack of the player (only used to display)
  "MATK": 10, //the max attack of the player (only used to display)
  "RT": 10, //the reload time of the player
  "PTS": 0, //the points of the player
  "KILL": 0 //the kills of the player
}
```

Setting the gamestate

```JSON
{
  "type": "gamestate",
  "gamestate": 0 // 0 -> Stopped, 1 -> Waiting, 2 -> Running
}
```

Request Device Information

```JSON
{
  "type": "information"
}
```

## Serial: ESP -> PC

On Device Change + On `get_devices` request

```JSON
{
  "type": "devices",
  "devices": [
    "192.168.1.1",
    "192.168.1.2",
  ]
}
```

On Error

```JSON
{
  "type": "error",
  "message": "Error Message"
}
```

On Message from Device, the Message will be sent as is

### Types of Messages from a Pistol

On hit

```JSON
{
  "type": "hit",
  "ip": "192.168.1.1", // ip of player that got hit
  "sender": "192.168.1.2" // ip of player that hit
}
```

Device info

```JSON
{
  "type":"device_information",
  "ip":"192.168.1.1",
  "device_type":"pistol",
  "firmware":"1.1.1"
}
```
