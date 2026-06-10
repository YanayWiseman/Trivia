using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TriviaClient
{
    internal class CryptoSession
    {
        public static string Username { get; set; }

        public static string ClientToServerKeyId { get; set; }
        public static string ServerToClientKeyId { get; set; }

        public static OTPCryptoAlgorithm RequestCrypto { get; set; }
        public static OTPCryptoAlgorithm ResponseCrypto { get; set; }

        public static bool IsReady
        {
            get
            {
                return RequestCrypto != null && ResponseCrypto != null;
            }
        }

        public static void Clear()
        {
            Username = null;
            ClientToServerKeyId = null;
            ServerToClientKeyId = null;
            RequestCrypto = null;
            ResponseCrypto = null;
        }
    }
}
