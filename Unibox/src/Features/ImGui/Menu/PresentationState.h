#pragma once
#include <algorithm>
#include <string>
#include <vector>

// Presentation metadata only. Feature values remain in the original ConfigVars.
namespace MenuPresentation
{
    inline constexpr int HomePage = 8; // Preserve existing page indices.
    inline constexpr size_t MaxFavorites = 64;
    struct State
    {
        std::vector<std::string> favorites;
        bool Contains(const std::string& key) const
        {
            return std::find(favorites.begin(), favorites.end(), key) != favorites.end();
        }
        bool Add(const std::string& key)
        {
            if (!key.starts_with("Vars::") || key.size() > 256 || Contains(key) || favorites.size() >= MaxFavorites)
                return false;
            favorites.push_back(key);
            return true;
        }
        bool Remove(const std::string& key)
        {
            const auto old = favorites.size();
            std::erase(favorites, key);
            return favorites.size() != old;
        }
    };
    inline State Preferences;
}
