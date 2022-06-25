#include <iostream>
#include "discord-files/discord.h"

// struct type to hold info about the games configuration
struct ezrp_config_t {

    // fields

    // Activity struct 
    discord::ClientId ApplicationId;
    const char* State;
    const char* Details;

    // ActivityTimestamps struct 
    discord::Timestamp Start;
    discord::Timestamp End;

    // ActivityAssets struct 
    const char* LargeImage;
    const char* LargeText;
    const char* SmallImage;
    const char* SmallText;

    // ActivityParty struct 
    const char* Id;

    // PartySize struct 
    int32_t CurrentSize;
    int32_t MaxSize;
    
    // ActivitySecrets struct 
    const char* Match;
    const char* Join;
    const char* Spectate;

    // ActivityType struct 
    discord::ActivityType _atenumv;

    // methods

    // true if presence needs to be updated
    bool changed = true;

    void update();
    void print();
};

extern struct config_t config;
