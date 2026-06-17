using System;

namespace TriviaClient
{
    internal class OTPCryptoAlgorithm : ICryptoAlgoithm
    {
        private readonly byte[] m_pad;
        private int m_offset;

        public int Offset
        {
            get
            {
                return this.m_offset;
            }
        }

        public byte[] Pad
        {
            get
            {
                return (byte[])this.m_pad.Clone();
            }
        }

        public OTPCryptoAlgorithm(byte[] pad, int offset)
        {
            if (pad == null || pad.Length == 0)
            {
                throw new ArgumentException("OTP pad cannot be empty");
            }

            if (offset < 0 || offset > pad.Length)
            {
                throw new ArgumentOutOfRangeException(nameof(offset));
            }

            this.m_pad = (byte[])pad.Clone();
            this.m_offset = offset;
        }

        public byte[] Encrypt(byte[] data)
        {
            if (data == null)
            {
                throw new ArgumentNullException(nameof(data));
            }

            if (this.m_offset + data.Length > this.m_pad.Length)
            {
                throw new Exception("OTP Encrypt: pad exhausted");
            }

            byte[] cipherText = new byte[data.Length];

            for (int i = 0; i < data.Length; i++)
            {
                cipherText[i] =
                    (byte)(data[i] ^ this.m_pad[this.m_offset + i]);
            }

            byte[] packet = BuildPacket(this.m_offset, cipherText);

            this.m_offset += data.Length;

            return packet;
        }

        public byte[] Decrypt(byte[] packet)
        {
            if (packet == null || packet.Length < 8)
            {
                throw new Exception("OTP Decrypt: packet too short");
            }

            int offset = ReadInt32(packet, 0);
            int cipherLength = ReadInt32(packet, 4);

            if (offset < 0 || cipherLength < 0)
            {
                throw new Exception("OTP Decrypt: invalid packet values");
            }

            if (packet.Length != 8 + cipherLength)
            {
                throw new Exception("OTP Decrypt: packet size mismatch");
            }

            if (offset != this.m_offset)
            {
                throw new Exception("OTP Decrypt: unexpected offset. Expected " + this.m_offset + " received " + offset);
            }

            if (offset + cipherLength > this.m_pad.Length)
            {
                throw new Exception("OTP Decrypt: pad exhausted or invalid offset");
            }

            byte[] plainText = new byte[cipherLength];

            for (int i = 0; i < cipherLength; i++)
            {
                plainText[i] =
                    (byte)(packet[8 + i] ^ this.m_pad[offset + i]);
            }

            this.m_offset += cipherLength;

            return plainText;
        }

        private byte[] BuildPacket(int offset, byte[] cipherText)
        {
            byte[] packet = new byte[8 + cipherText.Length];

            WriteInt32(packet, 0, offset);
            WriteInt32(packet, 4, cipherText.Length);

            Array.Copy(
                cipherText,
                0,
                packet,
                8,
                cipherText.Length);

            return packet;
        }

        private static void WriteInt32(byte[] buffer, int index, int value)
        {
            buffer[index] = (byte)((value >> 24) & 0xFF);
            buffer[index + 1] = (byte)((value >> 16) & 0xFF);
            buffer[index + 2] = (byte)((value >> 8) & 0xFF);
            buffer[index + 3] = (byte)(value & 0xFF);
        }

        private static int ReadInt32(byte[] buffer, int index)
        {
            return
                (buffer[index] << 24) |
                (buffer[index + 1] << 16) |
                (buffer[index + 2] << 8) |
                buffer[index + 3];
        }
    }
}