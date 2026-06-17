using System;
using System.IO;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace TriviaClient
{
    internal class Communicator
    {
        private const int PORT = 12334;
        private const byte PlainPacket = 0;
        private const byte EncryptedPacket = 1;
        private const int MAX_PACKET_SIZE = 4 * 1024 * 1024;

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
            This method should only be used for signup.
            Input: requestBuffer (byte[]) - The normal request packet.
            Output: The deserialized response.
        */

        public T SendRequest<T>(byte[] requestBuffer)
        {
            lock (this.m_lock)
            {
                byte[] outerRequest = BuildPlainPacket(requestBuffer);

                SendFramedPacket(outerRequest);

                byte[] outerResponse = ReceiveFramedPacket();
                byte[] normalResponse = ParsePlainResponse(outerResponse);

                return Deserializer.DeserializeResponse<T>(normalResponse);
            }
        }

        /*
            Method sends an OTP encrypted request to the server.
            This method should be used for login and every request after login.
            Input: requestBuffer (byte[]) - The normal request packet before encryption.
            Output: The deserialized decrypted response.
        */

        public T SendEncryptedRequest<T>(byte[] requestBuffer)
        {
            lock (this.m_lock)
            {
                if (!CryptoSession.IsReady || CryptoSession.RequestCrypto == null || CryptoSession.ResponseCrypto == null || CryptoSession.ClientToServerKeyId == null)
                {
                    throw new Exception("OTP crypto session is not ready");
                }

                byte[] encryptedRequest = CryptoSession.RequestCrypto.Encrypt(requestBuffer);
                byte[] outerRequest = BuildEncryptedRequest(CryptoSession.ClientToServerKeyId, encryptedRequest);

                SendFramedPacket(outerRequest);

                byte[] outerResponse = ReceiveFramedPacket();
                byte[] encryptedResponse = ParseEncryptedResponse(outerResponse);
                byte[] normalResponse = CryptoSession.ResponseCrypto.Decrypt(encryptedResponse);

                CryptoSession.Save();

                return Deserializer.DeserializeResponse<T>(normalResponse);
            }
        }

        /*
            Helper method builds a plaintext outer packet.
            Input: normalPacket (byte[]) - The original request packet.
            Output: Packet in format [mode][normal packet].
        */

        private byte[] BuildPlainPacket(byte[] normalPacket)
        {
            byte[] packet = new byte[1 + normalPacket.Length];

            packet[0] = PlainPacket;
            Array.Copy(normalPacket, 0, packet, 1, normalPacket.Length);

            return packet;
        }

        /*
            Helper method builds an encrypted outer request packet.
            Input: keyId (string), encryptedPacket (byte[])
            Output: Packet in format [mode][key id length][key id][OTP packet].
        */

        private byte[] BuildEncryptedRequest(string keyId, byte[] encryptedPacket)
        {
            byte[] keyIdBytes = Encoding.UTF8.GetBytes(keyId);

            if (keyIdBytes.Length > ushort.MaxValue)
            {
                throw new Exception("OTP key ID is too long");
            }

            byte[] packet = new byte[1 + 2 + keyIdBytes.Length + encryptedPacket.Length];
            int index = 0;

            packet[index++] = EncryptedPacket;
            packet[index++] = (byte)((keyIdBytes.Length >> 8) & 0xFF);
            packet[index++] = (byte)(keyIdBytes.Length & 0xFF);

            Array.Copy(keyIdBytes, 0, packet, index, keyIdBytes.Length);
            index += keyIdBytes.Length;
            Array.Copy(encryptedPacket, 0, packet, index, encryptedPacket.Length);

            return packet;
        }

        /*
            Helper method extracts a normal packet from a plaintext response.
            Input: outerPacket (byte[])
            Output: The normal response packet.
        */

        private byte[] ParsePlainResponse(byte[] outerPacket)
        {
            if (outerPacket.Length < 2)
            {
                throw new Exception("Server returned an invalid plaintext packet");
            }

            if (outerPacket[0] != PlainPacket)
            {
                throw new Exception("Expected a plaintext server response");
            }

            byte[] normalPacket = new byte[outerPacket.Length - 1];
            Array.Copy(outerPacket, 1, normalPacket, 0, normalPacket.Length);

            return normalPacket;
        }

        /*
            Helper method extracts an OTP packet from an encrypted response.
            Input: outerPacket (byte[])
            Output: The encrypted OTP packet.
        */

        private byte[] ParseEncryptedResponse(byte[] outerPacket)
        {
            if (outerPacket.Length < 2)
            {
                throw new Exception("Server returned an invalid encrypted packet");
            }

            if (outerPacket[0] != EncryptedPacket)
            {
                throw new Exception("Expected an encrypted server response");
            }

            byte[] encryptedPacket = new byte[outerPacket.Length - 1];
            Array.Copy(outerPacket, 1, encryptedPacket, 0, encryptedPacket.Length);

            return encryptedPacket;
        }

        /*
            Helper method sends a packet with a 4-byte length prefix.
            Input: packet (byte[])
            Output: no return values
        */

        private void SendFramedPacket(byte[] packet)
        {
            int packetLength = packet.Length;

            byte[] lengthBytes =
            {
                (byte)((packetLength >> 24) & 0xFF),
                (byte)((packetLength >> 16) & 0xFF),
                (byte)((packetLength >> 8) & 0xFF),
                (byte)(packetLength & 0xFF)
            };

            this.m_clientStream.Write(lengthBytes, 0, lengthBytes.Length);
            this.m_clientStream.Write(packet, 0, packet.Length);
            this.m_clientStream.Flush();
        }

        /*
            Helper method reads a packet with a 4-byte length prefix.
            Input: no input values
            Output: The received packet.
        */

        private byte[] ReceiveFramedPacket()
        {
            byte[] lengthBytes = ReadExactBytes(4);
            int packetLength = (lengthBytes[0] << 24) | (lengthBytes[1] << 16) | (lengthBytes[2] << 8) | lengthBytes[3];

            if (packetLength <= 0 || packetLength > MAX_PACKET_SIZE)
            {
                throw new Exception("Server returned an invalid packet size");
            }

            return ReadExactBytes(packetLength);
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

                if (bytesRead <= 0)
                {
                    throw new IOException("The server closed the connection");
                }

                totalRead += bytesRead;
            }

            return buffer;
        }
    }
}
