#include "profile.hpp"

#include <fstream>
#include <sstream>
#include <filesystem>

namespace profile
{
namespace
{
const char *DEFAULT_FILE = "profile.env";

PlayerProfile DefaultProfile()
{
    return PlayerProfile{};
}

std::string Trim(const std::string &value)
{
    size_t start = value.find_first_not_of(" \t\r\n");
    size_t end = value.find_last_not_of(" \t\r\n");
    if (start == std::string::npos)
        return {};
    return value.substr(start, end - start + 1);
}

std::string NormalizePath(const std::string &filePath)
{
    if (filePath.empty())
    {
        return DEFAULT_FILE;
    }
    return filePath;
}

} // namespace

bool ProfileFileExists(const std::string &filePath)
{
    std::error_code ec;
    return std::filesystem::exists(NormalizePath(filePath), ec);
}

static void EnsureDirectory(const std::filesystem::path &path)
{
    auto dir = path.parent_path();
    if (!dir.empty())
    {
        std::error_code ec;
        std::filesystem::create_directories(dir, ec);
    }
}

bool LoadProfile(const std::string &filePath, PlayerProfile &profile)
{
    auto path = NormalizePath(filePath);
    if (!ProfileFileExists(path))
    {
        profile = DefaultProfile();
        SaveProfile(path, profile);
        return true;
    }

    std::ifstream in(path, std::ios::in);
    if (!in.is_open())
    {
        profile = DefaultProfile();
        return false;
    }

    PlayerProfile loaded = DefaultProfile();
    std::string line;
    while (std::getline(in, line))
    {
        if (line.empty() || line[0] == '#')
            continue;
        auto pos = line.find('=');
        if (pos == std::string::npos)
            continue;
        std::string key = Trim(line.substr(0, pos));
        std::string value = Trim(line.substr(pos + 1));
        if (key == "PLAYER_NAME")
            loaded.name = value.empty() ? "Guest" : value;
        else if (key == "BANKROLL")
            loaded.bankroll = std::stoi(value);
        else if (key == "BLACKJACK_WINS")
            loaded.blackjackWins = std::stoi(value);
        else if (key == "BLACKJACK_LOSSES")
            loaded.blackjackLosses = std::stoi(value);
        else if (key == "BLACKJACK_PUSHES")
            loaded.blackjackPushes = std::stoi(value);
        else if (key == "ROULETTE_SPINS")
            loaded.rouletteSpins = std::stoi(value);
        else if (key == "ROULETTE_NET")
            loaded.rouletteNet = std::stoi(value);
    }

    profile = loaded;
    return true;
}

bool SaveProfile(const std::string &filePath, const PlayerProfile &profile)
{
    auto path = NormalizePath(filePath);
    EnsureDirectory(path);
    std::ofstream out(path, std::ios::out | std::ios::trunc);
    if (!out.is_open())
    {
        return false;
    }

    out << "PLAYER_NAME=" << profile.name << '\n';
    out << "BANKROLL=" << profile.bankroll << '\n';
    out << "BLACKJACK_WINS=" << profile.blackjackWins << '\n';
    out << "BLACKJACK_LOSSES=" << profile.blackjackLosses << '\n';
    out << "BLACKJACK_PUSHES=" << profile.blackjackPushes << '\n';
    out << "ROULETTE_SPINS=" << profile.rouletteSpins << '\n';
    out << "ROULETTE_NET=" << profile.rouletteNet << '\n';

    return true;
}

std::unordered_map<std::string, std::string> ToMap(const PlayerProfile &profile)
{
    return {
        { "PLAYER_NAME", profile.name },
        { "BANKROLL", std::to_string(profile.bankroll) },
        { "BLACKJACK_WINS", std::to_string(profile.blackjackWins) },
        { "BLACKJACK_LOSSES", std::to_string(profile.blackjackLosses) },
        { "BLACKJACK_PUSHES", std::to_string(profile.blackjackPushes) },
        { "ROULETTE_SPINS", std::to_string(profile.rouletteSpins) },
        { "ROULETTE_NET", std::to_string(profile.rouletteNet) }
    };
}

} // namespace profile
