#ifndef PLAYER_H
#define PLAYER_H

#include "json.h"
using namespace std;


class Player
{
    public:
        static const char* VERSION;

        static int betRequest(json::Value game_state);

        static void showdown(json::Value game_state);
		
};

#endif
