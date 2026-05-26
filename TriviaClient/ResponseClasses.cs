using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TriviaClient
{
    public class LoginResponse
    {
        // this means that when the deserializer reads the json, it looks for "status"
        // and not "Status" like the name suggests
        [JsonProperty("status")]
        public uint Status { get; set; }
    }

    public class SignupResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }
    }

    public class ErrorResponse
    {
        [JsonProperty("message")]
        public string? Message { get; set; }
    }

    public class LogoutResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }
    }

    public class GetRoomsResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }

        [JsonProperty("rooms")]
        public List<RoomData> Rooms { get; set; } = new List<RoomData>();
    }

    public class GetPlayersInRoomResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }

        [JsonProperty("PlayersInRoom")]
        public string? PlayersRaw { get; set; }

        // Method to convert the string into a list
        public List<string> GetPlayers()
        {
            if (string.IsNullOrEmpty(PlayersRaw))
            {
                return new List<string>();
            }

            return PlayersRaw.Split('&').ToList();
        }
    }

    public class GetHighScoreResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }

        [JsonProperty("HighScore")]
        public string? StatisticsRaw { get; set; }

        public List<string> GetHighScores()
        {
            if (string.IsNullOrEmpty(StatisticsRaw))
            {
                return new List<string>();
            }

            return StatisticsRaw.Split('&').ToList();
        }
    }

    public class GetPersonalStatsResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }


        [JsonProperty("UserStatistics")]
        public string? StatisticsRaw { get; set; }

        public List<string> GetStats()
        {
            if (string.IsNullOrEmpty(StatisticsRaw))
            {
                return new List<string>();
            }

            return StatisticsRaw.Split('&').ToList();
        }
    }

    public class JoinRoomResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }
    }

    public class LeaveRoomResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }
    }
    public class StartGameResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }
    }

    public class CloseRoomResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }
    }
    public class GetGameResultResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }

        [JsonProperty("results")]
        public List<ServerPlayerResult>? results { get; set; }
    }
    public class ServerPlayerResult
    {
        [JsonProperty("username")]
        public string? Username { get; set; }

        [JsonProperty("correctAnswerCount")]
        public int CorrectAnswerCount { get; set; }

        [JsonProperty("wrongAnswerCount")]
        public int WrongAnswerCount { get; set; }

        [JsonProperty("averageAnswerTime")]
        public double AverageAnswerTime { get; set; }
    }

    public class CreateRoomResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }

        [JsonProperty("roomId")]
        public uint RoomId { get; set; }
    }

    public class RoomData
    {
        [JsonProperty("status")]
        public uint Status { get; set; }

        [JsonProperty("id")]
        public uint Id { get; set; }

        [JsonProperty("name")]
        public string? Name { get; set; }

        [JsonProperty("maxPlayers")]
        public uint MaxPlayers { get; set; }

        [JsonProperty("numOfQuestionsInGame")]
        public uint NumOfQuestionsInGame { get; set; }

        [JsonProperty("timePerQuestion")]
        public uint TimePerQuestion { get; set; }

        [JsonProperty("roomStatus")]
        public RoomStatus roomStatus { get; set; }
    }


    public class GetRoomStateResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }

        [JsonProperty("hasGameBegun")]
        public RoomStatus roomStatus { get; set; }

        [JsonProperty("players")]
        public string? PlayerListRaw { get; set; }

        public List<string> GetPlayers()
        {
            if (string.IsNullOrEmpty(PlayerListRaw))
            {
                return new List<string>();
            }

            return PlayerListRaw.Split('&').ToList();
        }

        [JsonProperty("questionCount")]
        public uint NumOfQuestionsInGame { get; set; }

        [JsonProperty("timePerQuestion")]
        public uint TimePerQuestion { get; set; }
    }


    public enum RoomStatus
    {
        Open,
        Closed,
        InGame
    }
}
