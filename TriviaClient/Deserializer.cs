using Newtonsoft.Json;
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace TriviaClient
{
    internal static class Deserializer
    {
        /*
            Method to deserialize the server's response
            input: the buffer
            output: a template, one of the classes based on the response
        */
        public static T DeserializeResponse<T>(byte[] buffer)
        {
            string json = GetJson(buffer);

            T? returnValue = JsonConvert.DeserializeObject<T>(json);

            if (returnValue == null)
            {
                throw new Exception("Failed to deserialize response.");
            }

            return returnValue;
        }

        /*
            Method to get the json part of the buffer as a string
            input: the buffer
            output: the json part of the buffer
        */
        public static string GetJson(byte[] buffer)
        {
            int jsonSize = GetJsonSize(buffer);

            // decode the specific portion of a byte array
            // start from index 5 because of the headers
            return Encoding.UTF8.GetString(buffer, 5, jsonSize);
        }

        /*
            Method to get the json's size in the buffer
            input: the buffer
            output: the json's size in the buffer
        */
        public static int GetJsonSize(byte[] buffer)
        {
            if (buffer.Length < 5)
            {
                throw new Exception("Invalid packet length");
            }

            return (buffer[1] << 24) |
               (buffer[2] << 16) |
               (buffer[3] << 8) |
                buffer[4];
        }
    }
}

