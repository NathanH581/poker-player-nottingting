#include "player.h"
#include "json.h"
#include <iostream>

const char* Player::VERSION = "Default C++ player";

int Player::betRequest(json::Value game_state)
{
    try {
        std::cerr<<json::Serialize(game_state)<<std::endl;
        std::string s = json::Serialize(game_state);
        std::string delimiter = "},{";
        std::string token;
        size_t pos = 0;
        while ((pos = s.find(delimiter)) != std::string::npos) {
            token = s.substr(0,pos);
            //std::cerr << token << std::endl;
            s.erase(0, pos + delimiter.length());
        }
        
        if ( game_state.HasKey("minimum_raise") ) { 
            int minimum_raise = game_state["minimum_raise"].ToInt();
            std::cerr << "Here is minimum raise" << minimum_raise << std::endl;
        }
        
        int currentBet = game_state["current_buy_in");
        int potSize = game_state["pot"];
        std::cerr << "=========================================" << std::endl;
        json::Array players = game_state["players"].ToArray();
        int currentChipCount = 0;
        for(auto it= players.begin();it != players.end(); it++){
            json::Value player = (*it);
            if (player.HasKey("hole_cards")){
                json::Array hole_cards = player["hole_cards"];
                std::cerr<<"we can see cards in our hands!!!!!!!!!!!"<<std::endl;
                std::cerr<<json::Serialize(hole_cards)<<std::endl;
                if (player.HasKey("stack")) {
                    int currentChipCount = player["stack"];
                }
            }
        }
        
        json::Array community_cards = game_state["community_cards"].ToArray();
        std::cerr<<"we can see cards on table!!!!!!!!!!!!!!!!"<<std::endl;
        for(auto it2=community_cards.begin();it2 != community_cards.end(); it2++){
            std::cerr<<"iteration"<<std::endl;
            json::Value one_card = (*it2);
            std::string card_number = one_card["rank"].ToString();
            std::string card_suite = one_card["suit"].ToString();
            std::cerr << card_number << ",,," << card_suite << std::endl;
        }

        if (currentBet == 0) {
            return currentChipCount;
        } else {
            return 0;
        }
        
    } catch(const std::exception& e) {
        // in case it crashes
        return 0;
    }
}

void Player::showdown(json::Value game_state)
{
}
