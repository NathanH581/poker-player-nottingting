#include "player.h"
#include "json.h"
#include <iostream>
const char* Player::VERSION = "Default C++ folding player";

int Player::betRequest(json::Value game_state)
{
    std::cerr<<json::Serialize(game_state)<<std::endl;
    std::string s = json::Serialize(game_state);
    std::string delimiter = "},{";
    std::string token;
    size_t pos = 0;
    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0,pos);
        std::cout << token << std::endl;
        s.erase(0, pos + delimiter.length());
    }
    return 100;

}

void Player::showdown(json::Value game_state)
{
}
