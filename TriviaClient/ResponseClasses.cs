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
        public string Error { get; set; }
    }

    public class LogoutResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }
    }

    public class GetRoomsResponse
    {
        public uint Status { get; set; }
        public List<RoomData>? Rooms { get; set; }
    }

    public class GetPlayersInRoomResponse
    {
        public List<string>? Players { get; set; }
    }

    public class GetHighScoreResponse
    {
        public uint Status { get; set; }
        public List<string> Statistics { get; set; }
    }

    public class GetPersonalStatsResponse
    {
        public uint Status { get; set; }
        public List<string> Statistics { get; set; }
    }

    public class JoinRoomResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }
    }

    public class CreateRoomResponse
    {
        [JsonProperty("status")]
        public uint Status { get; set; }
    }

    public class RoomData
    {
        [JsonProperty("id")]
        public uint Id { get; set; }

        [JsonProperty("name")]
        public string Name { get; set; }

        [JsonProperty("maxPlayers")]
        public uint MaxPlayers { get; set; }

        [JsonProperty("numOfQuestionsInGame")]
        public uint NumOfQuestionsInGame { get; set; }

        [JsonProperty("timePerQuestion")]
        public uint TimePerQuestion { get; set; }

        [JsonProperty("status")]
        public RoomStatus Status { get; set; }
    }

    public enum RoomStatus
    {
        Open,
        Closed
    }
}
