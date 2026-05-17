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
        private enum RequestCode
        {
            LoginRequestCode = 100,
            SignupRequestCode,
            LogoutRequestCode,
            GetRoomsRequestCode,
            GetPlayersInRoomRequestCode,
            HighScoreRequestCode,
            GetPersonalStatsRequestCode,
            JoinRoomRequestCode,
            CreateRoomRequestCode,
            CloseRoomRequestCode,
            StartGameRequestCode,
            GetRoomStateRequestCode,
            LeaveRoomRequestCode
        }

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
            return SerializeJson(request, (int)RequestCode.LoginRequestCode);
        }


        /*
            Function Serealizes SignupRequest
            Input: request (SignupRequest)
            Output: Byte buffer (byte[])
        */
        public static byte[] serializeSignupRequest(SignupRequest request)
		{
            return SerializeJson(request, (int)RequestCode.SignupRequestCode);
        }


        /*
            Function Serealizes GetPlayersInRoomRequest
            Input: request (GetPlayersInRoomRequest)
            Output: Byte buffer (byte[])
        */

        public static byte[] serializeGetPlayersRequest(GetPlayersInRoomRequest request)
		{
            return SerializeJson(request, (int)RequestCode.GetPlayersInRoomRequestCode);
        }


        /*
            Function Serealizes JoinRoomRequest
            Input: request (JoinRoomRequest)
            Output: Byte buffer (byte[])
        */

        public static byte[] serializeJoinRoomRequest(JoinRoomRequest request)
		{
            return SerializeJson(request, (int)RequestCode.JoinRoomRequestCode);
        }


        /*
            Function Serealizes CreateRoomRequest
            Input: request (CreateRoomRequest)
            Output: Byte buffer (byte[])
        */

        public static byte[] serializeCreateRoomRequest(CreateRoomRequest request)
		{
            return SerializeJson(request, (int)RequestCode.CreateRoomRequestCode);
        }


        /*
            Function Serealizes LogoutRequest
            Input: None
            Output: Byte buffer (byte[])
        */
        public static byte[] SerializeLogoutRequest()
        {
            return SerializeJson(new { }, (int)RequestCode.LogoutRequestCode);
        }
        
        /*
         * Function serializes GetRoomsRequest
         * Input: None
         * Output: Byte buffer (byte[])
        */
        public static byte[] SerializeGetRoomsRequest()
        {
            return SerializeJson(new { }, (int)RequestCode.GetRoomsRequestCode);
        }

        public static byte[] SerializeHighScoreRequest()
        {
            return SerializeJson(new { }, (int)RequestCode.HighScoreRequestCode);
        }

        public static byte[] SerializeGetPersonalStatsRequest()
        {
            return SerializeJson(new { }, (int)RequestCode.GetPersonalStatsRequestCode);
        }
    }
}
