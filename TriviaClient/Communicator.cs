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
        private TcpClient m_tcpClient;

        public Communicator()
        {
            // create the tcp client and connect to the server
            this.m_tcpClient = new TcpClient();
            IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse("127.0.0.1"), 12334);
            m_tcpClient.Connect(serverEndPoint);
        }
    }
}
