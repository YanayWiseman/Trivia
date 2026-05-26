using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;

namespace TriviaClient
{
    public class LoginRequest
    {
        [JsonProperty("username")]
        public string Username { get; set; } = "";

        [JsonProperty("password")]
        public string Password { get; set; } = "";
    }

    public class SignupRequest
    {
        [JsonProperty("username")]
        public string Username { get; set; } = "";

        [JsonProperty("password")]
        public string Password { get; set; } = "";

        [JsonProperty("email")]
        public string Email { get; set; } = "";

        [JsonProperty("address")]
        public string Address { get; set; } = "";

        [JsonProperty("phone")]
        public string Phone { get; set; } = "";

        [JsonProperty("birthdate")]
        public string Birthdate { get; set; } = "";
    }

    public class GetPlayersInRoomRequest
    {
        [JsonProperty("roomId")]
        public uint RoomId { get; set; }
    }

    public class JoinRoomRequest
    {
        [JsonProperty("roomId")]
        public uint RoomId { get; set; }
    }

    public class CreateRoomRequest
    {
        [JsonProperty("roomName")]
        public string RoomName { get; set; } = "";

        [JsonProperty("maxUsers")]
        public uint MaxUsers { get; set; }

        [JsonProperty("questionsCount")]
        public uint QuestionsCount { get; set; }

        [JsonProperty("answerTimeout")]
        public uint AnswerTimeout { get; set; }
    }

     public class GetQuestionRequest
     {
     }

    public class SubmitAnswerRequest
    {
        [JsonProperty("answerId")]
        public uint AnswerId { get; set; }
    }

    public class GetGameResultsRequest
    {
    }

    public class LeaveGameRequest
    {
    }
}
