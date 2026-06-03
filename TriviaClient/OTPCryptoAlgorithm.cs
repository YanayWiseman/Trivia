using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace TriviaClient
{
    internal class OTPCryptoAlgorithm : ICryptoAlgoithm
    {
        private readonly Random m_rnd = new Random();
        public byte[] Encrypt(byte[] data)
        {
            // randomly generate the pad
            byte[] pad = new byte[data.Length];
            this.m_rnd.NextBytes(pad);   // fill it up with random values

            // first 4 bytes is the key length
            int keyLen = pad.Length;
            byte[] result = new byte[4 + keyLen + keyLen];

            result[0] = (byte)((keyLen >> 24) & 0xFF);
            result[1] = (byte)((keyLen >> 16) & 0xFF);
            result[2] = (byte)((keyLen >> 8) & 0xFF);
            result[3] = (byte)((keyLen) & 0xFF);

            Array.Copy(pad, 0, result, 4, keyLen);

            int i = 0;
            for (i = 0; i < data.Length; i++)
            {
                result[4 + keyLen + i] = (byte)(data[i] ^ pad[i]);  // xor
            }

            return result;
        }

        public byte[] Decrypt(byte[] data)
        {

        }
    }
}
