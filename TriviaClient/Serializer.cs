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
        private const int LoginRequestCode = 1;
        private const int SignupRequestCode = 3;
        private const int LogoutRequestCode = 13;
        private const int GetRoomsRequestCode = 14;
        private const int GetPlayersInRoomRequestCode = 15;
        private const int HighScoreRequestCode = 16;
        private const int GetPersonalStatsRequestCode = 17;
        private const int JoinRoomRequestCode = 18;
        private const int CreateRoomRequestCode = 19;


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
            return SerializeJson(request, LoginRequestCode);
        }
        /*
            Function Serealizes SignupRequest
            Input: request (SignupRequest)
            Output: Byte buffer (byte[])
        */
        public static byte[] serializeSignupRequest(SignupRequest request)
		{
            return SerializeJson(request, SignupRequestCode);
        }
        /*
            Function Serealizes GetPlayersInRoomRequest
            Input: request (GetPlayersInRoomRequest)
            Output: Byte buffer (byte[])
        */
        public static byte[] serializeGetPlayersRequest(GetPlayersInRoomRequest request)
		{
            return SerializeJson(request, GetPlayersInRoomRequestCode);
        }
        /*
            Function Serealizes JoinRoomRequest
            Input: request (JoinRoomRequest)
            Output: Byte buffer (byte[])
        */
        public static byte[] serializeJoinRoomRequest(JoinRoomRequest request)
		{
            return SerializeJson(request, JoinRoomRequestCode);
        }
        /*
            Function Serealizes CreateRoomRequest
            Input: request (CreateRoomRequest)
            Output: Byte buffer (byte[])
        */
        public static byte[] serializeCreateRoomRequest(CreateRoomRequest request)
		{
            return SerializeJson(request, CreateRoomRequestCode);
        }
    }
}
