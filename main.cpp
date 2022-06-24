#include "discord-files/discord.h"
#include <csignal>
#include <ctime>
#include <iostream>
#include <thread>
#include <chrono>

#define SUCCEEDED(dr) (((discord::Result)(dr)) == discord::Result::Ok) // sue me
#define FAILED(dr) (((discord::Result)(dr)) != discord::Result::Ok)

//struct DiscordState {
//    std::unique_ptr<discord::Core> core;
//};

volatile std::sig_atomic_t interrupted = 0;

void got_interrupted(int sig) {
    interrupted = 1;
}

int main() {
    std::cout << "starting..." << std::endl;

    std::signal(SIGINT, got_interrupted);
    std::signal(SIGTERM, got_interrupted);
#ifdef _WIN32
    std::signal(SIGBREAK, got_interrupted);
#else
    std::signal(SIGHUP, got_interrupted);
#endif

    discord::Core* core{};
    discord::Result dres;

    discord::ClientId appId = 0;
    dres = discord::Core::Create(appId, DiscordCreateFlags_Default, &core);
    if FAILED(dres) {
        std::cout << "Could not create a Discord instance";
        return 1;
    }
    discord::Activity activity{};

    //int time_right_now = std::chrono::seconds(std::time(0)).count();

    //activity.SetState("Testing");
    //activity.SetDetails("Fruit Loops");
    activity.GetAssets().SetLargeImage("kurisu_large");
    activity.GetAssets().SetLargeText(":)");
    activity.GetTimestamps().SetStart(std::time(0)); // better than std::chrono::seconds(std::time(0)).count() because I don't get a C24244
    discord::PartySize partySize{};
    partySize.SetCurrentSize(1);
    //partySize.SetMaxSize(2^(8 * sizeof(int32_t)) - 1);
    partySize.SetMaxSize(5);
    discord::ActivityParty activityParty{};
    activityParty.SetId("pid");
    discord::ActivitySecrets activitySecrets{};
    activitySecrets.SetJoin("sid");
    activity.SetType(discord::ActivityType::Playing);
   

    core->ActivityManager().UpdateActivity(activity, [](discord::Result dres) {
        if (SUCCEEDED(dres)) {}
        std::cout << "updating activity..." << std::endl;
    });

    core->ActivityManager().RegisterCommand("cmd.exe");

    while (!interrupted) {
        core->RunCallbacks();
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }

    std::cout << "closing..." << std::endl; // not sure if this is neccessary, but I might want to expand this to a custom closing sequence in the future
    core->ActivityManager().ClearActivity([](discord::Result dres) {});
}

// https://discord.com/developers/docs/game-sdk/sdk-starter-guide
// https://discord.com/developers/docs/topics/gateway#activity-object-activity-types
// https://github.com/msciotti/discord-game-sdk-test-apps/blob/master/cpp-examples/example-presence/example-presence/main.cpp