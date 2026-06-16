using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;


namespace TriviaClient
{
    public partial class VerificationPage : Page
    {
        private readonly string email;
        private readonly TaskCompletionSource<bool> resultSource = new TaskCompletionSource<bool>();

        public VerificationPage(string email)
        {
            InitializeComponent();

            this.email = email;

            SendCode();
        }

        public Task<bool> WaitForResultAsync()
        {
            return resultSource.Task;
        }

        private async void SendCode()
        {
            try
            {
                ResendButton.IsEnabled = false;

                await EmailVerification.SendCode(email);

                StatusTextBlock.Text = "Code sent to " + email;
            }
            catch (Exception ex)
            {
                StatusTextBlock.Text = "Failed to send code: " + ex.Message;
            }
            finally
            {
                ResendButton.IsEnabled = true;
            }
        }

        private void VerifyButton_Click(object sender, RoutedEventArgs e)
        {
            string code = CodeTextBox.Text.Trim();

            if (code == "")
            {
                StatusTextBlock.Text = "Enter the verification code.";
                return;
            }

            bool verified = EmailVerification.CheckCode(email, code);

            if (verified)
            {
                resultSource.TrySetResult(true);
            }
            else
            {
                StatusTextBlock.Text = "Wrong or expired code.";
            }
        }

        private void ResendButton_Click(object sender, RoutedEventArgs e)
        {
            SendCode();
        }

        private void GoBack_Click(object sender, RoutedEventArgs e)
        {
            resultSource.TrySetResult(false);
        }
    }
}