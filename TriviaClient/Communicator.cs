using System;
using System.Linq;
using System.Net;
using System.Net.Sockets;

namespace TriviaClient
{
    internal class Communicator
    {
        private const int PORT = 12334;

        private TcpClient m_tcpClient;
        private NetworkStream m_clientStream;

        private readonly object m_lock = new object();

        /*
            Constructor method to construct the communicator object.
            Input: no input values
            Output: no return values
        */
        public Communicator()
        {
            this.m_tcpClient = new TcpClient();
            IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), PORT);

            this.m_tcpClient.Connect(serverEndPoint);
            this.m_clientStream = this.m_tcpClient.GetStream();
        }

        /*
            Method disconnects the communicator.
            Input: no input values
            Output: no return values
        */
        public void Disconnect()
        {
            this.m_clientStream.Close();
            this.m_tcpClient.Close();
        }

        /*
            Method sends a normal unencrypted request to the server.
            Input: requestBuffer (byte[]) - The normal request packet.
            Output: The deserialized response.
        */
        public T SendRequest<T>(byte[] requestBuffer)
        {
            lock (this.m_lock)
            {
                SendRawPacket(requestBuffer);

                byte[] response = GetRawPacket();

                return Deserializer.DeserializeResponse<T>(response);
            }
        }

        /*
            Method sends an OTP encrypted request to the server.
            Input: requestBuffer (byte[]) - The normal request packet before encryption.
            Output: The deserialized decrypted response.
        */
        public T SendEncryptedRequest<T>(byte[] requestBuffer)
        {
            lock (this.m_lock)
            {
                if (!CryptoSession.IsReady)
                {
                    throw new Exception("OTP crypto session is not ready");
                }

                byte[] encryptedRequest = EncryptNormalPacketBody(requestBuffer, CryptoSession.RequestCrypto);

                SendRawPacket(encryptedRequest);

                byte[] encryptedResponse = GetRawPacket();

                byte[] decryptedResponse = DecryptNormalPacketBody(encryptedResponse, CryptoSession.ResponseCrypto);

                return Deserializer.DeserializeResponse<T>(decryptedResponse);
            }
        }

        /*
            Helper method sends a raw packet to the server.
            Input: packet (byte[]) - The full packet to send.
            Output: no return values
        */
        private void SendRawPacket(byte[] packet)
        {
            int len = packet.Length;

            byte[] lenBytes = new byte[]
            {
                (byte)((len >> 24) & 0xFF),
                (byte)((len >> 16) & 0xFF),
                (byte)((len >> 8) & 0xFF),
                (byte)(len & 0xFF)
            };

            byte[] toSend = lenBytes.Concat(packet).ToArray();

            this.m_clientStream.Write(toSend, 0, toSend.Length);
        }

        /*
            Helper method reads a raw packet from the server.
            Input: no input values
            Output: The raw packet as a byte array.
        */
        private byte[] GetRawPacket()
        {
            byte[] lenBytes = ReadExactBytes(4);

            int len =
                (lenBytes[0] << 24) |
                (lenBytes[1] << 16) |
                (lenBytes[2] << 8) |
                lenBytes[3];

            return ReadExactBytes(len);
        }

        /*
            Helper method encrypts only the body of a normal protocol packet.
            Input: normalPacket (byte[]), crypto (OTPCryptoAlgorithm)
            Output: Packet in format [code][encrypted length][encrypted body].
        */
        private byte[] EncryptNormalPacketBody(byte[] normalPacket, OTPCryptoAlgorithm crypto)
        {
            if (normalPacket.Length < 5)
            {
                throw new Exception("Packet too short");
            }

            byte code = normalPacket[0];

            int bodyLength =
                (normalPacket[1] << 24) |
                (normalPacket[2] << 16) |
                (normalPacket[3] << 8) |
                normalPacket[4];

            if (normalPacket.Length != 5 + bodyLength)
            {
                throw new Exception("Packet length mismatch");
            }

            byte[] body = new byte[bodyLength];

            int i = 0;
            for (i = 0; i < bodyLength; i++)
            {
                body[i] = normalPacket[5 + i];
            }

            byte[] encryptedBody = crypto.Encrypt(body);

            return BuildNormalPacket(code, encryptedBody);
        }

        /*
            Helper method decrypts only the body of a normal protocol packet.
            Input: encryptedPacket (byte[]), crypto (OTPCryptoAlgorithm)
            Output: Packet in format [code][decrypted length][decrypted body].
        */
        private byte[] DecryptNormalPacketBody(byte[] encryptedPacket, OTPCryptoAlgorithm crypto)
        {
            if (encryptedPacket.Length < 5)
            {
                throw new Exception("Packet too short");
            }

            byte code = encryptedPacket[0];

            int bodyLength =
                (encryptedPacket[1] << 24) |
                (encryptedPacket[2] << 16) |
                (encryptedPacket[3] << 8) |
                encryptedPacket[4];

            if (encryptedPacket.Length != 5 + bodyLength)
            {
                throw new Exception("Packet length mismatch");
            }

            byte[] encryptedBody = new byte[bodyLength];

            int i = 0;
            for (i = 0; i < bodyLength; i++)
            {
                encryptedBody[i] = encryptedPacket[5 + i];
            }

            byte[] decryptedBody = crypto.Decrypt(encryptedBody);

            return BuildNormalPacket(code, decryptedBody);
        }

        /*
            Helper method builds a normal protocol packet.
            Input: code (byte), body (byte[])
            Output: Packet in format [code][length][body].
        */
        private byte[] BuildNormalPacket(byte code, byte[] body)
        {
            byte[] packet = new byte[5 + body.Length];

            packet[0] = code;

            int length = body.Length;
            packet[1] = (byte)((length >> 24) & 0xFF);
            packet[2] = (byte)((length >> 16) & 0xFF);
            packet[3] = (byte)((length >> 8) & 0xFF);
            packet[4] = (byte)(length & 0xFF);

            int i = 0;
            for (i = 0; i < body.Length; i++)
            {
                packet[5 + i] = body[i];
            }

            return packet;
        }

        /*
            Helper method reads the exact amount of bytes from the socket stream.
            Input: bytesAmount (int) - The amount of bytes to read.
            Output: A byte array containing exactly the requested amount of bytes.
        */
        private byte[] ReadExactBytes(int bytesAmount)
        {
            byte[] buffer = new byte[bytesAmount];

            int totalRead = 0;

            while (totalRead < bytesAmount)
            {
                int bytesRead = this.m_clientStream.Read(buffer, totalRead, bytesAmount - totalRead);

                if (bytesRead == 0)
                {
                    throw new Exception("Failed to read server response");
                }

                totalRead += bytesRead;
            }

            return buffer;
        }
    }
}