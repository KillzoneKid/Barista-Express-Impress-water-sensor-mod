#include "player.h"
#include "sensor.h"

void setup()
{
    Player::setup();
    Sensor::setup();
}

void loop() 
{
    Player::loop(millis());
    Sensor::loop(millis());
}