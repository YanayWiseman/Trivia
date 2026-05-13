using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TriviaClient
{
    internal static class Deserializer
    {
        /*
            Method to deserialize the server's response
            input: the buffer
            output: a template, one of the classes based on the response
        */
        public static T DeserializeResponse<T>(byte[] buffer)
        {
            string json = GetJson(buffer);
        }

        /*
            Method to get the json part of the buffer as a string
            input: the buffer
            output: the json part of the buffer
        */
        public static string GetJson(byte[] buffer)
        {

        }

        /*
            Method to get the json's size in the buffer
            input: the buffer
            output: the json's size in the buffer
        */
        public static int getJsonSize(byte[] buffer)
        {
            return (buffer[1] << 24) |
               (buffer[2] << 16) |
               (buffer[3] << 8) |
                buffer[4];
        }
        
        /*
            Method to get the response code
            input: the buffer
            output: the resoponse code
        */
        public static byte GetResponseCode(byte[] buffer)
        {
            return buffer[0];
        }

    }
    public class LoginResponse
    {
        public uint Status { get; set; }
    }

    public class SignupResponse
    {
        public uint Status { get; set; }
    }

    public class ErrorResponse
    {
        public string Error { get; set; }
    }

    public class LogoutResponse
    {
        public uint Status { get; set; }
    }

    public class GetRoomsResponse
    {
        public uint Status { get; set; }
        public List<RoomData>? Rooms { get; set; }
    }

    public class GetPlayersInRoomResponse
    {
        public List<string> Players { get; set; }
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
        public uint Status { get; set; }
    }

    public class CreateRoomResponse
    {
        public uint Status { get; set; }
    }

    public class RoomData
    {
        public uint Id { get; set; }
        public string Name { get; set; }
        public uint MaxPlayers { get; set; }
        public uint NumOfQuestionsInGame { get; set; }
        public uint TimePerQuestion { get; set; }
        public RoomStatus Status { get; set; }
    }

    public enum RoomStatus
    {
        Open,
        Closed
    }
}

