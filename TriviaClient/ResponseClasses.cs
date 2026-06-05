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
        public uint Status { get; set; }
        public string clientToServerKeyId { get; set; }
        public string serverToClientKeyId { get; set; }
        public string clientToServerKey { get; set; }
        public string serverToClientKey { get; set; }
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
        public List<string> Players { get; set; } = new();
    }

    public class GetHighScoreResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }

        [JsonProperty("HighScore")]
        public List<string> Statistics { get; set; } = new();
    }

    public class GetPersonalStatsResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }


        [JsonProperty("UserStatistics")]
        public List<string> Statistics { get; set; } = new();
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
        public List<string> Players { get; set; } = new();

        [JsonProperty("questionCount")]
        public uint NumOfQuestionsInGame { get; set; }

        [JsonProperty("timePerQuestion")]
        public uint TimePerQuestion { get; set; }
    }

    public class GetQuestionResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }

        [JsonProperty("question")]
        public string Question { get; set; } = "";

        [JsonProperty("answers")]
        public Dictionary<uint, string> Answers { get; set; } = new Dictionary<uint, string>();
    }

    public class SubmitAnswerResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }

        [JsonProperty("correctAnsId")]
        public uint CorrectAnswerId { get; set; }
    }

    public class LeaveGameResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }
    }

    public class PlayerResults
    {
        [JsonProperty("username")]
        public string Username { get; set; } = "";

        [JsonProperty("correctAnswerCount")]
        public uint CorrectAnswerCount { get; set; }

        [JsonProperty("wrongAnswerCount")]
        public uint WrongAnswerCount { get; set; }

        [JsonProperty("avrageAnswerTime")]
        public uint AverageAnswerTime { get; set; }
    }

    public class GetGameResultsResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }

        [JsonProperty("results")]
        public List<PlayerResults> Results { get; set; } = new List<PlayerResults>();
    }

    public class OneVsOneStatusResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }

        [JsonProperty("queueStatus")]
        public uint QueueStatus { get; set; }

        [JsonProperty("roomId")]
        public uint RoomId { get; set; }

        [JsonProperty("gameId")]
        public uint GameId { get; set; }

        [JsonProperty("questionsCount")]
        public uint QuestionsCount { get; set; }

        [JsonProperty("answerTimeout")]
        public uint AnswerTimeout { get; set; }

        [JsonProperty("secondsUntilStart")]
        public uint SecondsUntilStart { get; set; }

        [JsonProperty("opponentUsername")]
        public string OpponentUsername { get; set; } = "";
    }


    public class AddQuestionResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }
    }


    public enum RoomStatus
    {
        Open,
        Closed,
        InGame
    }

    public enum OneVsOneQueueStatus
    {
        OneVsOneNotQueued = 0,
        OneVsOneWaiting = 1,
        OneVsOneMatched = 2
    }
}
