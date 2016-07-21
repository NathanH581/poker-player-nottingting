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

/* 
    bb = small_blind *2
    return stackSize/bb;
*/
int numberOfBigBlinds(int bb, int stackSize) {
	return (stackSize/bb);
}
	
	bool judge_better(int &bb,std::vector<Card> &cards){
/*
    Final Hand Teirs
    20 BB or more
        AA,KK,QQ,JJ,1010
        AK,AQ,AJ
*/
	Card a = cards.front();
	Card b = cards.back();
	if (a.rank < b.rank){
		Card c = a;
		a = b;
		b = c;
	}
	
	
	if (a.rank == b.rank){
		if (b.rank == "J" || b.rank == "Q" || b.rank == "K" || b.rank == "10" || b.rank == "A"){
			return true;
		}
	}
	
	if (a.rank == "A"){
		if (b.rank == "J" || b.rank == "Q" || b.rank == "K"){
			return true;
		}
	}
	
	if (bb >= 5){
		/*
    between 5 and 15 BB (inclusive)
        AA,KK,QQ,JJ,1010
        AK,AQ,AJ
        99,88,77,66,55
        A and anything
        KQ, KJ, QJ
        AA,KK,QQ,JJ,1010
        AK,AQ,AJ
        99,88,77,66,55

        A and anything
        KQ, KJ, QJ
		*/
		if (a.rank == "A" || a.rank == b.rank){
			return true;
		}
		
		if (a.rank == "K"){
			if (b.rank == "J" || b.rank == "Q"){
				return true;
			}
		}
		
		if (a.rank == "Q" && b.rank == "J"){
			return true;
		}
		
	}else{
		if (a.rank == "A" || a.rank == "K" || a.rank == "Q"){
			return true;
		}
		/*
    Less than 5 BB
                AA,KK,QQ,JJ,1010
        AK,AQ,AJ
        99,88,77,66,55

        A and anything
        K and anything
        Q and anything
*/

	}
		return false;
	}
	
int judge(std::vector<Card> &cards){
	Card a = cards.front();
	Card b = cards.back();
		if (b.rank < a.rank){
		Card c = a;
		a = b;
		b = c;
	}
	// Pair
	if (a.rank == b.rank){
			if (a.rank == "10" || a.rank == "J" || a.rank == "Q" || a.rank == "K" || a.rank == "A"){
				return 1;
			}
	}
	
	if (b.rank == "A"){
		if (a.rank == "10" || a.rank == "J" || a.rank == "Q" || a.rank == "K"){
		return 1;
		}
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
        
        if ( game_state.HasKey("minimum_raise") ) { 
            int minimum_raise = game_state["minimum_raise"].ToInt();
            std::cerr << "Here is minimum raise" << minimum_raise << std::endl;
        }
        
        if ( game_state.HasKey("small_blind") ) { 
            int small_blind = game_state["small_blind"].ToInt();
            std::cerr << "Here is small_blind" << small_blind << std::endl;
        }
        
        int current_buy_in = 0;
		if ( game_state.HasKey("current_buy_in") ) { 
            current_buy_in = game_state["current_buy_in"].ToInt();
            std::cerr << "Here is current_buy_in " << current_buy_in << std::endl;
        }
		
        int currentBet = game_state["current_buy_in"].ToInt();
        int potSize = game_state["pot"].ToInt();
		int our_bet = 0;
        std::cerr << "=========================================" << std::endl;
        json::Array players = game_state["players"].ToArray();
        int currentChipCount = 0;
		std::vector<Card> cards;
        for(auto it= players.begin();it != players.end(); it++){
            json::Value player = (*it);
			if ( player.HasKey("bet") ) { 
				our_bet = player["bet"].ToInt();
				std::cerr << "Here is bet " << our_bet << std::endl;
			}
            if (player.HasKey("hole_cards")){
                json::Array hole_cards = player["hole_cards"];
                std::cerr<<"we can see cards in our hands!!!!!!!!!!!"<<std::endl;
                std::cerr<<json::Serialize(hole_cards)<<std::endl;
                if (player.HasKey("stack")) {
                    currentChipCount = player["stack"].ToInt();
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
	    cerr << std::endl;
	 }// end print our hole cards
        
        json::Array community_cards = game_state["community_cards"].ToArray();
        std::cerr<<"we can see cards on table!!!!!!!!!!!!!!!!"<<std::endl;
		std::vector<Card> community;
        for(auto it2=community_cards.begin();it2 != community_cards.end(); it2++){
            std::cerr<<"iteration"<<std::endl;
            json::Value one_card = (*it2);
            std::string card_number = one_card["rank"].ToString();
            std::string card_suite = one_card["suit"].ToString();
			Card card;
			card.suite = card_suite;
			card.rank = card_number;
			community.push_back(card);
            std::cerr << card_number << ",,," << card_suite << std::endl;
        }
		int val = current_buy_in - our_bet;
		if (judge(cards)){
			std::cerr << "We are positive from judge with " << currentChipCount << std::endl;
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
