using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MailKit.Net.Smtp;
using MailKit.Security;
using MimeKit;
using System.Security.Cryptography;
using System.Windows;


namespace TriviaClient
{
    internal static class EmailVerification
    {
        private static string currentEmail = "";
        private static string currentCode = "";
        private static DateTime expirationTime;

        public static async Task SendCode(string email)
        {
            string senderEmail = Environment.GetEnvironmentVariable("TRIVIA_EMAIL");
            string appPassword = Environment.GetEnvironmentVariable("TRIVIA_EMAIL_PASSWORD");

            if (senderEmail == null || appPassword == null)
            {
                throw new Exception("Email environment variables are missing.");
            }

            currentEmail = email;
            currentCode = RandomNumberGenerator.GetInt32(0, 1000000).ToString("D6");
            expirationTime = DateTime.Now.AddMinutes(10);

            MimeMessage message = new MimeMessage();
            message.From.Add(new MailboxAddress("Trivia Game", senderEmail));
            message.To.Add(MailboxAddress.Parse(email));
            message.Subject = "Trivia verification code";
            message.Body = new TextPart("plain")
            {
                Text = "Your Trivia verification code is: " + currentCode + "\n\nThe code expires in 10 minutes."
            };

            using SmtpClient client = new SmtpClient();

            await client.ConnectAsync("smtp.gmail.com", 587, SecureSocketOptions.StartTls);
            await client.AuthenticateAsync(senderEmail, appPassword);
            await client.SendAsync(message);
            await client.DisconnectAsync(true);
        }

        public static bool CheckCode(string email, string code)
        {
            if (email != currentEmail)
            {
                return false;
            }

            if (DateTime.Now > expirationTime)
            {
                return false;
            }

            return code == currentCode;
        }
    }
}