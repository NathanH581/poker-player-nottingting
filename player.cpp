#include "player.h"
#include "json.h"
const char* Player::VERSION = "Default C++ folding player";

int Player::betRequest(json::Value game_state)
{
    return 100;
}

void Player::showdown(json::Value game_state)
{
}
