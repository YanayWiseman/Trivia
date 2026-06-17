using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TriviaClient
{
    class ClientSession
    {
        public static Communicator Communicator { get; set; } = new Communicator();
    }
}
