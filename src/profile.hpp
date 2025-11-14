#ifndef PROFILE_HPP
#define PROFILE_HPP

#include <string>
#include <unordered_map>

namespace profile
{

struct PlayerProfile
{
    std::string name = "Guest";
    int bankroll = 1000;
    int blackjackWins = 0;
    int blackjackLosses = 0;
    int blackjackPushes = 0;
    int rouletteSpins = 0;
    int rouletteNet = 0;
};

// Loads a profile from the given env-style file path. Creates defaults if missing.
bool LoadProfile(const std::string &filePath, PlayerProfile &profile);

// Saves the profile to the env-style file path.
bool SaveProfile(const std::string &filePath, const PlayerProfile &profile);

// Returns true if a file exists at the path.
bool ProfileFileExists(const std::string &filePath);

// Converts profile to key/value pairs for display or debugging.
std::unordered_map<std::string, std::string> ToMap(const PlayerProfile &profile);

} // namespace profile

#endif // PROFILE_HPP
