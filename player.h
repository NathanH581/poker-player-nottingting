#ifndef PLAYER_H
#define PLAYER_H

#include "json.h"


class Player
{
    public:
        static const char* VERSION;

        static int betRequest(json::Value game_state);

        static void showdown(json::Value game_state);
		
	private:
		class Card{
				std::string suite;
				std::string rank;
		};
		
		std::vector<Card> m_cards;
};

#endif
