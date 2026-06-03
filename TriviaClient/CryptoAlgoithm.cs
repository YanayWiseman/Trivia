using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace TriviaClient
{
    internal interface ICryptoAlgoithm
    {
        byte[] Encrypt(byte[] data); 
        byte[] Decrypt(byte[] data);
    }
}
