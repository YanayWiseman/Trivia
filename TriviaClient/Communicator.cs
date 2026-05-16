using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace TriviaClient
{
    internal class Communicator
    {
        private const int PORT = 12334;

        private TcpClient m_tcpClient;
        private NetworkStream m_clientStream;

        // mutex
        private readonly object m_lock = new object();

        /*
            Constructor method to construct the communicator object
            input: no input values
            output: no return valus
        */
        public Communicator()
        {
            // create the tcp client and connect to the server
            this.m_tcpClient = new TcpClient();
            IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), PORT);

            m_tcpClient.Connect(serverEndPoint);
            this.m_clientStream = this.m_tcpClient.GetStream();
        }

        /*
            Method to disconnect the communicator
            input: no input values
            output: no return valus
        */
        public void Disconnect()
        {
            this.m_clientStream.Close();
            this.m_tcpClient.Close();
        }

        /*
            Method to send a request to the server and get the deserialized response
            input: the request buffer as a byte array
            output: the deserialized response (Template)
        */
        public T SendRequest<T>(byte[] requestBuffer)
        {
            lock (this.m_lock)
            {
                // send request and recieve response
                SendToServer(requestBuffer);

                byte[] response = GetResponse();

                // deserialize the response
                return Deserializer.DeserializeResponse<T>(response);
            }
        }

        /*
            Helper method to send a request to the server
            input: the request buffer as a byte array
            output: no return values
        */
        private void SendToServer(byte[] requestBuffer)
        {
            this.m_clientStream.Write(requestBuffer);
        }

        /*
            Helper method to get the response from the server
            input: no input values
            output:the response buffer as a byte array 
        */
        private byte[] GetResponse()
        {
            // first read the headers
            byte[] headers = ReadExactBytes(5);

            // get the json's size
            int jsonSize = 
                (headers[1] << 24) |
                (headers[2] << 16) |
                (headers[3] << 8) |
                 headers[4];

            // read exact amount that the response says
            byte[] response = ReadExactBytes(jsonSize);

            // add it to the headers, convert to byte array and return
            return headers.Concat(response).ToArray();
        }

        /*
            Helper method to read the exact amount of bytes asked for from the socket stream
            input: the size to read
            output: a byte array the size requested
        */
        private byte[] ReadExactBytes(int bytesAmount)
        {
            byte[] buffer = new byte[bytesAmount];

            int totalRead = 0;

            // while we still dont have all of the bytes
            while (totalRead < bytesAmount)
            {
                // TCP gurentees whole of the packet will make it, in order
                // but it doesnt mean that it will be in one go, so we iterate it
                // TCP is a stream.
                int bytesRead = this.m_clientStream.Read(buffer, totalRead, bytesAmount - totalRead);
                if (bytesRead == 0)
                {
                    throw new Exception("Failed to read server's response");
                }

                totalRead += bytesRead;
            }

            return buffer;
        }
    }
}
