using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TriviaClient
{
    internal static class Serializer
	{
        private const int LoginRequest = 1;
        private const int SignupRequest = 3;
        private const int LogoutRequest = 13;
        private const int GetRoomsRequest = 14;
        private const int GetPlayersInRoomRequest = 15;
        private const int HighScoreRequest = 16;
        private const int GetPersonalStatsRequest = 17;
        private const int JoinRoomRequest = 18;
        private const int CreateRoomRequest = 19;


        /*
            Function Serializes JSON object into buffer.
            Input: obj (object), code (int)
            Output: Byte buffer (byte[])
        */

        public static byte[] SerializeJson(object obj, int code)
        {
            string jsonStr = JsonConvert.SerializeObject(obj);

            byte[] jsonBytes = Encoding.UTF8.GetBytes(jsonStr);

            int size = jsonBytes.Length;

            byte[] buffer = new byte[1 + 4 + size];

            buffer[0] = (byte)code;

            buffer[1] = (byte)((size >> 24) & 0xFF);
            buffer[2] = (byte)((size >> 16) & 0xFF);
            buffer[3] = (byte)((size >> 8) & 0xFF);
            buffer[4] = (byte)(size & 0xFF);

            int i = 0;
            for (i = 0; i < size; i++)
            {
                buffer[5 + i] = jsonBytes[i];
            }

            return buffer;
        }

        /*
            Function Serealizes login request.
            Input: request (LoginRequest)
            Output: Byte buffer (byte[])
        */

        public static byte[] SerializeLoginRequest(LoginRequest request)
		{
            return SerializeJson(request, LoginRequest);
        }
        /*
            Function Serealizes SignupRequest
            Input: request (SignupRequest)
            Output: Byte buffer (byte[])
        */
        public static byte[] serializeSignupRequest(SignupRequest request)
		{
            return SerializeJson(request, SignupRequest);
        }
        /*
            Function Serealizes GetPlayersInRoomRequest
            Input: request (GetPlayersInRoomRequest)
            Output: Byte buffer (byte[])
        */
        public static byte[] serializeGetPlayersRequest(GetPlayersInRoomRequest request)
		{
            return SerializeJson(request, GetPlayersInRoomRequest);
        }
        /*
            Function Serealizes JoinRoomRequest
            Input: request (JoinRoomRequest)
            Output: Byte buffer (byte[])
        */
        public static byte[] serializeJoinRoomRequest(JoinRoomRequest request)
		{
            return SerializeJson(request, JoinRoomRequest);
        }
        /*
            Function Serealizes CreateRoomRequest
            Input: request (CreateRoomRequest)
            Output: Byte buffer (byte[])
        */
        public static byte[] serializeCreateRoomRequest(CreateRoomRequest request)
		{
            return SerializeJson(request, CreateRoomRequest);
        }
    }
}
