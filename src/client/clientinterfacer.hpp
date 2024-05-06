#ifndef __CLIENTINTERFACER_HPP__
#define __CLIENTINTERFACER_HPP__

#pragma warning(push)
#pragma warning(disable:4505)

#include <string>
#include <utils.hpp>

#include "interface/holder.hpp"
#include "managers/connectormanager.hpp"
#include "endpointmappers/champselectsession.hpp"

#undef interface

#define CIGetRequest clientinterfacer::GetRequest
#define CIGetAssignedPosition clientinterfacer::GetAssignedPosition
#define CIDodgeQueue clientinterfacer::DodgeQueue
#define CIPostRequest clientinterfacer::PostRequest

namespace clientinterfacer {
    static const std::vector<std::string> lanes = { "top", "jungle", "middle", "bottom", "utility" };
    static const std::vector<std::string> divisions = { "I", "II", "III", "IV" };
    static const std::vector<std::string> tiers = { "iron", "bronze", "silver", "gold", "platinum", "emerald", "diamond", "master", "grandmaster", "challenger" };

    namespace queues {
        static const std::string rankedSolo = "RANKED_SOLO_5x5";
        static const std::string rankedFlex = "RANKED_FLEX_SR";
    }; // namespace queues

    static auto IdsToLanesConvertor = [](const int& id) -> std::string { return lanes[id]; };
    static auto LanesToIdsConvertor = [](const std::string& lane) { return static_cast<int>(std::ranges::distance(lanes.begin(), std::ranges::find(lanes, lane))); };

    template <typename Ty>
    static std::optional<Ty> GetRequest(const std::string& endpoint, int succesCode = 200) {
        if (const auto result = interface<ConnectorManager>::Get()->MakeRequest(connector::request_type::GET, endpoint); result.status == succesCode)
            return result.data.get<Ty>();
            
        return std::nullopt;
    }

    static bool PostRequest(const std::string& endpoint, int succesCode = 204) {
        return interface<ConnectorManager>::Get()->MakeRequest(connector::request_type::POST, endpoint).status == succesCode;
    }

    static bool PostRequest(const std::string& endpoint, const std::string& data, int succesCode = 204) {
        return interface<ConnectorManager>::Get()->MakeRequest(connector::request_type::POST, endpoint, data).status == succesCode;
    }

    static std::string GetAssignedPosition(const champselect::Session& session) {
        auto myTeamIterator = std::ranges::find_if(session.myTeam.value(), [session](const champselect::MyTeam& obj) {
            return obj.cellId.value() == session.localPlayerCellId.value();
        });

        if (myTeamIterator == session.myTeam->end())
            return "";

        return myTeamIterator->assignedPosition.value();
    }

    static bool DodgeQueue() {
        const auto result = interface<ConnectorManager>::Get()->MakeRequest(connector::request_type::POST, "/lol-login/v1/session/invoke?destination=lcdsServiceProxy&method=call&args=[\"\",\"teambuilder-draft\",\"quitV2\",\"\"]", "[\"\",\"teambuilder-draft\",\"quitV2\",\"\"]");
        return result.status == 200;
    }
}; // namespace clientinterfacer

#pragma warning(pop)

#endif // __CLIENTINTERFACER_HPP__