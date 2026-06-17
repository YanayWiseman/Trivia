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
using System;


namespace TriviaClient
{
    public partial class LoginPage : Page
    {
        public LoginPage()
        {
            InitializeComponent();
        }

        private void GoBack_Click(object sender, RoutedEventArgs e)
        {
            if (this.NavigationService.CanGoBack)
            {
                this.NavigationService.GoBack();
            }
        }

        /*
            Function attempts to log the user into the server.
            Input: sender (object), e (RoutedEventArgs)
            Output: None
        */

        private void LoginButton_Click(object sender, RoutedEventArgs e)
        {
            string username = UsernameTextBox.Text.Trim();
            string password = PasswordBox.Password;

            if (username == "" || password == "")
            {
                MessageBox.Show("Username and password cannot be empty.");
                return;
            }

            /*
                The user must already have saved OTP pads on this device.
                The pads were received and saved during signup.
            */

            if (!CryptoSession.Load(username))
            {
                MessageBox.Show("No encryption keys were found for this user. This user must sign up on this device first.");
                return;
            }

            LoginRequest request = new LoginRequest
            {
                Username = username,
                Password = password
            };

            try
            {
                byte[] requestBuffer = Serializer.SerializeLoginRequest(request);
                LoginResponse response = ClientSession.Communicator.SendEncryptedRequest<LoginResponse>(requestBuffer);

                if (response.Status == 1)
                {
                    this.NavigationService.Navigate(new MenuPage(username));
                }
                else
                {
                    MessageBox.Show("Login failed.");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Encrypted login failed: " + ex.Message);
            }
        }
    }
}
