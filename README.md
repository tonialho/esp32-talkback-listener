# esp32-talkback-listener
ESP32 for fetching a value from TalkBack and acting on it

My use case

When I move a Trello card to the "Completed" pile, Trello sends a PUT command to Thingspeak TalkBack service. The TalkBack app has a single "command" (practically a key-value pair plus a position number), whose Command String (value) is affected by the Trello's PUT command. The value is changed to "true". 

ESP32 GETs the command string (true of false) by a Command ID and acts on it. If the value is "true", ESP32 runs the given code and goes on a hard coded cooldown. During the ESP32's cooldown, Trello sends another PUT command (after a hard coded time perioid), resetting the TalkBack value to "false".

The ESP32 could do anything imaginable for a microcontroller, whenever a Trello card is moved to the "Completed" pile. If you want to motivate a code monkey, for example, make your ESP32 control a fruit box(?) and drop a banana after a completion of a Trello card.