using Newtonsoft.Json;
using System;
using System.IO;

namespace TriviaClient
{
    internal static class CryptoSession
    {
        private class StoredCryptoSession
        {
            public string Username { get; set; } = "";

            public string ClientToServerKeyId { get; set; } = "";
            public string ServerToClientKeyId { get; set; } = "";

            public string ClientToServerPad { get; set; } = "";
            public string ServerToClientPad { get; set; } = "";

            public int ClientToServerOffset { get; set; }
            public int ServerToClientOffset { get; set; }
        }

        public static string? Username { get; set; }

        public static string? ClientToServerKeyId { get; set; }
        public static string? ServerToClientKeyId { get; set; }

        public static OTPCryptoAlgorithm? RequestCrypto { get; set; }
        public static OTPCryptoAlgorithm? ResponseCrypto { get; set; }

        public static bool IsReady
        {
            get
            {
                return !string.IsNullOrWhiteSpace(Username) && !string.IsNullOrWhiteSpace(ClientToServerKeyId) && !string.IsNullOrWhiteSpace(ServerToClientKeyId) && RequestCrypto != null && ResponseCrypto != null;
            }
        }

        public static void Initialize(string username, string clientToServerKeyId, string serverToClientKeyId, byte[] clientToServerPad, byte[] serverToClientPad, int clientToServerOffset = 0, int serverToClientOffset = 0)
        {
            Username = username;

            ClientToServerKeyId = clientToServerKeyId;
            ServerToClientKeyId = serverToClientKeyId;

            RequestCrypto = new OTPCryptoAlgorithm(clientToServerPad, clientToServerOffset);
            ResponseCrypto = new OTPCryptoAlgorithm(serverToClientPad, serverToClientOffset);
        }

        public static void Save()
        {
            if (!IsReady)
            {
                throw new Exception(
                    "Cannot save an incomplete crypto session");
            }

            StoredCryptoSession stored = new StoredCryptoSession
            {
                Username = Username!,
                ClientToServerKeyId = ClientToServerKeyId!,
                ServerToClientKeyId = ServerToClientKeyId!,
                ClientToServerPad = Convert.ToBase64String(RequestCrypto!.Pad),
                ServerToClientPad = Convert.ToBase64String(ResponseCrypto!.Pad),
                ClientToServerOffset = RequestCrypto.Offset,
                ServerToClientOffset = ResponseCrypto.Offset
            };

            string json = JsonConvert.SerializeObject(stored, Formatting.Indented);

            string path = GetSessionPath(Username!);

            Directory.CreateDirectory(Path.GetDirectoryName(path)!);

            File.WriteAllText(path, json);
        }

        public static bool Load(string username)
        {
            Clear();

            string path = GetSessionPath(username);

            if (!File.Exists(path))
            {
                return false;
            }

            try
            {
                string json = File.ReadAllText(path);

                StoredCryptoSession? stored = JsonConvert.DeserializeObject<StoredCryptoSession>(json);

                if (stored == null)
                {
                    return false;
                }

                Initialize(stored.Username, stored.ClientToServerKeyId,   stored.ServerToClientKeyId, Convert.FromBase64String(stored.ClientToServerPad), Convert.FromBase64String(stored.ServerToClientPad), stored.ClientToServerOffset, stored.ServerToClientOffset);

                return true;
            }
            catch
            {
                Clear();
                return false;
            }
        }

        public static void Delete(string username)
        {
            string path = GetSessionPath(username);

            if (File.Exists(path))
            {
                File.Delete(path);
            }

            if (Username == username)
            {
                Clear();
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

        private static string GetSessionPath(string username)
        {
            string safeUsername = MakeSafeFileName(username);

            return Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.LocalApplicationData), "TriviaClient", "Crypto",  safeUsername + ".json");
        }

        private static string MakeSafeFileName(string value)
        {
            foreach (char invalidChar in Path.GetInvalidFileNameChars())
            {
                value = value.Replace(invalidChar, '_');
            }

            return value;
        }
    }
}