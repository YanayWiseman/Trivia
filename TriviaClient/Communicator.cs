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
        NetworkStream m_clientStream;

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
            this.m_tcpClient.Close();
            this.m_clientStream.Close();
        }

        /*
            Method to send a request to the server and get the deserialized response
            input: the request buffer as a byte array
            output: the deserialized response (Template)
        */
        public T SendRequest<T>(byte[] requestBuffer)
        {
            // send request and recieve response
            SendToServer(requestBuffer);

            byte[] response = GetResponse();

            // deserialize the response
            return Deserializer.DeserializeResponse<T>(response);
        }

        /*
            Method to send a request to the server
            input: the request buffer as a byte array
            output: no return values
        */
        private void SendToServer(byte[] requestBuffer)
        {
            this.m_clientStream.Write(requestBuffer);
        }

        /*
            Method to get the response from the server
            input: no input values
            output:the response buffer as a byte array 
        */
        private byte[] GetResponse()
        {
            byte[] response = new byte[4096];
            int bytesRead = this.m_clientStream.Read(response, 0, 4096);

            return response;
        }


    }
}
