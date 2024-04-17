#ifndef __SUMMARYPLAYERDATA_HPP__
#define __SUMMARYPLAYERDATA_HPP__

#include "helper.hpp"

namespace challenges {
    using nlohmann::json;

    #ifndef NLOHMANN_UNTYPED_challenges_HELPER
    #define NLOHMANN_UNTYPED_challenges_HELPER
    inline json get_untyped(const json & j, const char * property) {
        if (j.find(property) != j.end()) {
            return j.at(property).get<json>();
        }
        return json();
    }

    inline json get_untyped(const json & j, std::string property) {
        return get_untyped(j, property.data());
    }
    #endif

    #ifndef NLOHMANN_OPTIONAL_challenges_HELPER
    #define NLOHMANN_OPTIONAL_challenges_HELPER
    template <typename T>
    inline std::shared_ptr<T> get_heap_optional(const json & j, const char * property) {
        auto it = j.find(property);
        if (it != j.end() && !it->is_null()) {
            return j.at(property).get<std::shared_ptr<T>>();
        }
        return std::shared_ptr<T>();
    }

    template <typename T>
    inline std::shared_ptr<T> get_heap_optional(const json & j, std::string property) {
        return get_heap_optional<T>(j, property.data());
    }
    template <typename T>
    inline std::optional<T> get_stack_optional(const json & j, const char * property) {
        auto it = j.find(property);
        if (it != j.end() && !it->is_null()) {
            return j.at(property).get<std::optional<T>>();
        }
        return std::optional<T>();
    }

    template <typename T>
    inline std::optional<T> get_stack_optional(const json & j, std::string property) {
        return get_stack_optional<T>(j, property.data());
    }
    #endif

    struct CategoryProgress {
        std::optional<std::string> category;
        std::optional<int64_t> current;
        std::optional<std::string> level;
        std::optional<int64_t> max;
        std::optional<double> positionPercentile;
    };

    struct LevelToIconPath {
        std::optional<std::string> bronze;
        std::optional<std::string> challenger;
        std::optional<std::string> diamond;
        std::optional<std::string> gold;
        std::optional<std::string> grandmaster;
        std::optional<std::string> iron;
        std::optional<std::string> master;
        std::optional<std::string> platinum;
        std::optional<std::string> silver;
    };

    struct ChallengeTitleData {
        std::optional<std::string> challengeDescription;
        std::optional<int64_t> challengeId;
        std::optional<std::string> challengeName;
        std::optional<std::string> level;
        std::optional<LevelToIconPath> levelToIconPath;
    };

    struct Title {
        std::optional<std::string> backgroundImagePath;
        std::optional<ChallengeTitleData> challengeTitleData;
        std::optional<std::string> contentId;
        std::optional<std::string> iconPath;
        std::optional<bool> isPermanentTitle;
        std::optional<int64_t> itemId;
        std::optional<std::string> name;
        std::optional<std::string> purchaseDate;
        std::optional<std::string> titleAcquisitionName;
        std::optional<std::string> titleAcquisitionType;
        std::optional<std::string> titleRequirementDescription;
    };

    struct SummaryPlayerData {
        std::optional<int64_t> apexLadderUpdateTime;
        std::optional<int64_t> apexLeaderboardPosition;
        std::optional<std::string> bannerId;
        std::optional<std::vector<CategoryProgress>> categoryProgress;
        std::optional<std::string> crestId;
        std::optional<bool> isApex;
        std::optional<std::string> overallChallengeLevel;
        std::optional<int64_t> pointsUntilNextRank;
        std::optional<double> positionPercentile;
        std::optional<int64_t> prestigeCrestBorderLevel;
        std::optional<std::string> selectedChallengesString;
        std::optional<Title> title;
        std::optional<std::vector<nlohmann::json>> topChallenges;
        std::optional<int64_t> totalChallengeScore;
    };
}

namespace challenges {
    void from_json(const json & j, CategoryProgress & x);
    void to_json(json & j, const CategoryProgress & x);

    void from_json(const json & j, LevelToIconPath & x);
    void to_json(json & j, const LevelToIconPath & x);

    void from_json(const json & j, ChallengeTitleData & x);
    void to_json(json & j, const ChallengeTitleData & x);

    void from_json(const json & j, Title & x);
    void to_json(json & j, const Title & x);

    void from_json(const json & j, SummaryPlayerData & x);
    void to_json(json & j, const SummaryPlayerData & x);

    inline void from_json(const json & j, CategoryProgress& x) {
        x.category = get_stack_optional<std::string>(j, "category");
        x.current = get_stack_optional<int64_t>(j, "current");
        x.level = get_stack_optional<std::string>(j, "level");
        x.max = get_stack_optional<int64_t>(j, "max");
        x.positionPercentile = get_stack_optional<double>(j, "positionPercentile");
    }

    inline void to_json(json & j, const CategoryProgress & x) {
        j = json::object();
        j["category"] = x.category;
        j["current"] = x.current;
        j["level"] = x.level;
        j["max"] = x.max;
        j["positionPercentile"] = x.positionPercentile;
    }

    inline void from_json(const json & j, LevelToIconPath& x) {
        x.bronze = get_stack_optional<std::string>(j, "BRONZE");
        x.challenger = get_stack_optional<std::string>(j, "CHALLENGER");
        x.diamond = get_stack_optional<std::string>(j, "DIAMOND");
        x.gold = get_stack_optional<std::string>(j, "GOLD");
        x.grandmaster = get_stack_optional<std::string>(j, "GRANDMASTER");
        x.iron = get_stack_optional<std::string>(j, "IRON");
        x.master = get_stack_optional<std::string>(j, "MASTER");
        x.platinum = get_stack_optional<std::string>(j, "PLATINUM");
        x.silver = get_stack_optional<std::string>(j, "SILVER");
    }

    inline void to_json(json & j, const LevelToIconPath & x) {
        j = json::object();
        j["BRONZE"] = x.bronze;
        j["CHALLENGER"] = x.challenger;
        j["DIAMOND"] = x.diamond;
        j["GOLD"] = x.gold;
        j["GRANDMASTER"] = x.grandmaster;
        j["IRON"] = x.iron;
        j["MASTER"] = x.master;
        j["PLATINUM"] = x.platinum;
        j["SILVER"] = x.silver;
    }

    inline void from_json(const json & j, ChallengeTitleData& x) {
        x.challengeDescription = get_stack_optional<std::string>(j, "challengeDescription");
        x.challengeId = get_stack_optional<int64_t>(j, "challengeId");
        x.challengeName = get_stack_optional<std::string>(j, "challengeName");
        x.level = get_stack_optional<std::string>(j, "level");
        x.levelToIconPath = get_stack_optional<LevelToIconPath>(j, "levelToIconPath");
    }

    inline void to_json(json & j, const ChallengeTitleData & x) {
        j = json::object();
        j["challengeDescription"] = x.challengeDescription;
        j["challengeId"] = x.challengeId;
        j["challengeName"] = x.challengeName;
        j["level"] = x.level;
        j["levelToIconPath"] = x.levelToIconPath;
    }

    inline void from_json(const json & j, Title& x) {
        x.backgroundImagePath = get_stack_optional<std::string>(j, "backgroundImagePath");
        x.challengeTitleData = get_stack_optional<ChallengeTitleData>(j, "challengeTitleData");
        x.contentId = get_stack_optional<std::string>(j, "contentId");
        x.iconPath = get_stack_optional<std::string>(j, "iconPath");
        x.isPermanentTitle = get_stack_optional<bool>(j, "isPermanentTitle");
        x.itemId = get_stack_optional<int64_t>(j, "itemId");
        x.name = get_stack_optional<std::string>(j, "name");
        x.purchaseDate = get_stack_optional<std::string>(j, "purchaseDate");
        x.titleAcquisitionName = get_stack_optional<std::string>(j, "titleAcquisitionName");
        x.titleAcquisitionType = get_stack_optional<std::string>(j, "titleAcquisitionType");
        x.titleRequirementDescription = get_stack_optional<std::string>(j, "titleRequirementDescription");
    }

    inline void to_json(json & j, const Title & x) {
        j = json::object();
        j["backgroundImagePath"] = x.backgroundImagePath;
        j["challengeTitleData"] = x.challengeTitleData;
        j["contentId"] = x.contentId;
        j["iconPath"] = x.iconPath;
        j["isPermanentTitle"] = x.isPermanentTitle;
        j["itemId"] = x.itemId;
        j["name"] = x.name;
        j["purchaseDate"] = x.purchaseDate;
        j["titleAcquisitionName"] = x.titleAcquisitionName;
        j["titleAcquisitionType"] = x.titleAcquisitionType;
        j["titleRequirementDescription"] = x.titleRequirementDescription;
    }

    inline void from_json(const json & j, SummaryPlayerData& x) {
        x.apexLadderUpdateTime = get_stack_optional<int64_t>(j, "apexLadderUpdateTime");
        x.apexLeaderboardPosition = get_stack_optional<int64_t>(j, "apexLeaderboardPosition");
        x.bannerId = get_stack_optional<std::string>(j, "bannerId");
        x.categoryProgress = get_stack_optional<std::vector<CategoryProgress>>(j, "categoryProgress");
        x.crestId = get_stack_optional<std::string>(j, "crestId");
        x.isApex = get_stack_optional<bool>(j, "isApex");
        x.overallChallengeLevel = get_stack_optional<std::string>(j, "overallChallengeLevel");
        x.pointsUntilNextRank = get_stack_optional<int64_t>(j, "pointsUntilNextRank");
        x.positionPercentile = get_stack_optional<double>(j, "positionPercentile");
        x.prestigeCrestBorderLevel = get_stack_optional<int64_t>(j, "prestigeCrestBorderLevel");
        x.selectedChallengesString = get_stack_optional<std::string>(j, "selectedChallengesString");
        x.title = get_stack_optional<Title>(j, "title");
        x.topChallenges = get_stack_optional<std::vector<nlohmann::json>>(j, "topChallenges");
        x.totalChallengeScore = get_stack_optional<int64_t>(j, "totalChallengeScore");
    }

    inline void to_json(json & j, const SummaryPlayerData & x) {
        j = json::object();
        j["apexLadderUpdateTime"] = x.apexLadderUpdateTime;
        j["apexLeaderboardPosition"] = x.apexLeaderboardPosition;
        j["bannerId"] = x.bannerId;
        j["categoryProgress"] = x.categoryProgress;
        j["crestId"] = x.crestId;
        j["isApex"] = x.isApex;
        j["overallChallengeLevel"] = x.overallChallengeLevel;
        j["pointsUntilNextRank"] = x.pointsUntilNextRank;
        j["positionPercentile"] = x.positionPercentile;
        j["prestigeCrestBorderLevel"] = x.prestigeCrestBorderLevel;
        j["selectedChallengesString"] = x.selectedChallengesString;
        j["title"] = x.title;
        j["topChallenges"] = x.topChallenges;
        j["totalChallengeScore"] = x.totalChallengeScore;
    }
}

#endif // __SUMMARYPLAYERDATA_HPP__