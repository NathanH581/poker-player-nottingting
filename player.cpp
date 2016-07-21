#include "player.h"
#include "json.h"
#include <iostream>
#include <cstdlib>

const char* Player::VERSION = "Default C++ player";

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
    int r = ((double) rand() / (RAND_MAX)) + 1
    if (r == 1) {
        return 1000;
    } else {
        return 0;
    }

}

void Player::showdown(json::Value game_state)
{
}
