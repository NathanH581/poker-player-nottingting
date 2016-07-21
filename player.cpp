#include "player.h"
#include "json.h"
#include <iostream>
const char* Player::VERSION = "Default C++ folding player";

int Player::betRequest(json::Value game_state)
{
    std::cerr<<json::Serialize(game_state);
    return 100;
}

void Player::showdown(json::Value game_state)
{
}
