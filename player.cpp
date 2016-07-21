#include "player.h"
#include "json.h"
#include <iostream>
#include <map>
#include <vector>
#include <string>

using namespace std;

struct Card{
	std::string suite;
	std::string rank;
};
			
int judge(std::vector<Card> &cards){
	Card a = cards.front();
	Card b = cards.back();
	// Pair
	if (a.rank == b.rank){
			if (a.rank == "10" || a.rank == "J" || a.rank == "Q" || a.rank == "K" || a.rank == "A"){
				return 1;
			}
	}
	
	if (a.rank == "A" || b.rank == "A"){
		return 1;
	}
	
	// If more than 50% of other players are out
	
	return 0;
}


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
		std::vector<Card> cards;
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
					Card card;
					card.suite = suit;
					card.rank = rank;
					cards.push_back(card);
		        }
                // end get hole cards
            }
        }
        // print our hole cards
        std::cerr << "see our cards ======> " << std::endl;
	for (auto it = map_holecards.begin(); it != map_holecards.end() ;it++) {
	    std::cerr << "suit: " <<  it->first << std::endl;
	    for ( auto it_vec = it->second.begin(); it_vec != it->second.end(); it_vec++) {
		 cerr << "cards: " << *it_vec;
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
		if (judge(cards)){
			std::cerr << "We are positive from judge" << std::endl;
			return currentChipCount;
		}else{
			std::cerr << "We are negative from judge" << std::endl;
			return 0;
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
