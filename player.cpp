#include "player.h"
#include "json.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;

const char* Player::VERSION = "Default C++ player";

int Player::betRequest(json::Value game_state)
{
    try {
        std::map<string, vector<string> > map_holecards;
	    //map<string, vector<string> > map_community;
	
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
        
        int currentBet = game_state["current_buy_in"].ToInt();
        int potSize = game_state["pot"].ToInt();
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
                    int currentChipCount = player["stack"].ToInt();
                }
                
                // get hole cards
                for ( auto iter_holecards = hole_cards.begin(); iter_holecards != hole_cards.end(); iter_holecards++ ) {
		            string suit = (*iter_holecards)["suit"].ToString();
		            string rank = (*iter_holecards)["rank"].ToString();
		            if ( map_holecards.find(suit) != map_holecards.end() ) {
			            vector<string> vec;
		    	        vec.push_back(rank);
			            map_holecards.insert(make_pair(suit, vec));
		            }
		            else {
			            map_holecards[suit].push_back(rank);
		            }
		        }
                // end get hole cards
            }
        }
        // print our hole cards
        std::cerr << "see our cards ======> " << std::endl;
	for (auto it = map_holecards.begin(); it != map_holecards.end() ;it++) {
	    std::cerr << "suit: " <<  it->first << std::endl;
	    for ( auto it_vec = it->second.begin(); it_vec != it->second.end(); it_vec++) {
		 cerr << "cards: " << *it_vec << endl;
	    }
	 }// end print our hole cards
        
        json::Array community_cards = game_state["community_cards"].ToArray();
        std::cerr<<"we can see cards on table!!!!!!!!!!!!!!!!"<<std::endl;
        for(auto it2=community_cards.begin();it2 != community_cards.end(); it2++){
            std::cerr<<"iteration"<<std::endl;
            json::Value one_card = (*it2);
            std::string card_number = one_card["rank"].ToString();
            std::string card_suite = one_card["suit"].ToString();
            std::cerr << card_number << ",,," << card_suite << std::endl;
        }
        
        cards_to_map(community_cards, map_community);

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

void Player::cards_to_map(json::Array& cards, std::map<std::string,std::vector<string> >& map){

    // get hole cards
    for ( auto it = cards.begin(); it != cards.end(); it++ ) {
        string suit = (*it)["suit"].ToString();
        string rank = (*it)["rank"].ToString();
        if ( map.find(suit) != map.end() ) {
            vector<string> vec;
            vec.push_back(rank);
            map.insert(make_pair(suit, vec));
        } else {
            map[suit].push_back(rank);
        }
    }
    // end get hole cards
    // print our hole cards
    std::cerr << "see our cards ======> " << std::endl;
    for (auto it = map.begin(); it != map.end() ;it++) {
        std::cerr << "suit: " <<  it->first << std::endl;
        for ( auto it_vec = it->second.begin(); it_vec != it->second.end(); it_vec++) {
            cerr << "cards: " << *it_vec;
        }
    }// end print our hole cards
}
