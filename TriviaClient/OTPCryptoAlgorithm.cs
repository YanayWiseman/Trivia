using System;

namespace TriviaClient
{
    internal class OTPCryptoAlgorithm : ICryptoAlgoithm
    {
        private readonly byte[] m_pad;
        private int m_offset;

        public OTPCryptoAlgorithm(byte[] pad, int offset)
        {
            this.m_pad = pad;
            this.m_offset = offset;
        }

        public byte[] Encrypt(byte[] data)
        {
            if (this.m_offset + data.Length > this.m_pad.Length)
            {
                throw new Exception("OTP Encrypt: pad exhausted");
            }

            byte[] cipherText = new byte[data.Length];

            int i = 0;
            for (i = 0; i < data.Length; i++)
            {
                cipherText[i] = (byte)(data[i] ^ this.m_pad[this.m_offset + i]);
            }

            byte[] packet = BuildPacket(this.m_offset, cipherText);

            this.m_offset += data.Length;

            return packet;
        }

        public byte[] Decrypt(byte[] packet)
        {
            if (packet.Length < 8)
            {
                throw new Exception("OTP Decrypt: packet too short");
            }

            int offset = ReadInt32(packet, 0);
            int cipherLen = ReadInt32(packet, 4);

            if (packet.Length != 8 + cipherLen)
            {
                throw new Exception("OTP Decrypt: packet size mismatch");
            }

            if (offset + cipherLen > this.m_pad.Length)
            {
                throw new Exception("OTP Decrypt: pad exhausted or invalid offset");
            }

            if (offset != this.m_offset)
            {
                throw new Exception("OTP Decrypt: unexpected offset");
            }

            byte[] plainText = new byte[cipherLen];

            int i = 0;
            for (i = 0; i < cipherLen; i++)
            {
                plainText[i] = (byte)(packet[8 + i] ^ this.m_pad[offset + i]);
            }

            this.m_offset += cipherLen;

            return plainText;
        }

        private byte[] BuildPacket(int offset, byte[] cipherText)
        {
            byte[] packet = new byte[8 + cipherText.Length];

            WriteInt32(packet, 0, offset);
            WriteInt32(packet, 4, cipherText.Length);

            int i = 0;
            for (i = 0; i < cipherText.Length; i++)
            {
                packet[8 + i] = cipherText[i];
            }

            return packet;
        }

        private void WriteInt32(byte[] buffer, int index, int value)
        {
            buffer[index] = (byte)((value >> 24) & 0xFF);
            buffer[index + 1] = (byte)((value >> 16) & 0xFF);
            buffer[index + 2] = (byte)((value >> 8) & 0xFF);
            buffer[index + 3] = (byte)(value & 0xFF);
        }

        private int ReadInt32(byte[] buffer, int index)
        {
            return
                (buffer[index] << 24) |
                (buffer[index + 1] << 16) |
                (buffer[index + 2] << 8) |
                buffer[index + 3];
        }
    }
}