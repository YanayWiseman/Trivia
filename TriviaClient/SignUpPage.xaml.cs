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
    public partial class SignUpPage : Page
    {
        public SignUpPage()
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
            Function signs the user up and saves the OTP pads received from the server.
            Input: sender (object), e (RoutedEventArgs)
            Output: None
        */

        private async void GoToMenu_Click(object sender, RoutedEventArgs e)
        {
            string username = UsernameTextBox.Text;
            string password = PasswordTextBox.Text;
            string email = EmailTextBox.Text;
            string address = AddressTextBox.Text;
            string phone = PhoneTextBox.Text;
            string birthdate = "";

            if (BirthDatePicker.SelectedDate != null)
            {
                birthdate = BirthDatePicker.SelectedDate.Value.ToString("dd/MM/yyyy");
            }

            if (username == "" || password == "" || email == "" || address == "" || phone == "" || birthdate == "")
            {
                MessageBox.Show("Please fill all fields.");
                return;
            }

            bool verified = await VerifyEmail(email);

            if (!verified)
            {
                return;
            }

            SignupRequest request = new SignupRequest
            {
                Username = username,
                Password = password,
                Email = email,
                Address = address,
                Phone = phone,
                Birthdate = birthdate
            };

            try
            {
                byte[] requestBuffer = Serializer.serializeSignupRequest(request);

                SignupResponse response = ClientSession.Communicator.SendRequest<SignupResponse>(requestBuffer);

                if (response.Status == 1)
                {
                    byte[] clientToServerPad = Convert.FromBase64String(response.ClientToServerKey);
                    byte[] serverToClientPad = Convert.FromBase64String(response.ServerToClientKey);

                    CryptoSession.Initialize(username, response.ClientToServerKeyId, response.ServerToClientKeyId, clientToServerPad, serverToClientPad);
                    CryptoSession.Save();

                    MessageBox.Show("Signup succeeded. Encryption keys were saved.");
                    NavigationService.Navigate(new LoginPage());
                }
                else
                {
                    MessageBox.Show("Signup failed.");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Server error: " + ex.Message);
            }
        }

        private async Task<bool> VerifyEmail(string email)
        {
            VerificationPage verificationPage = new VerificationPage(email);
            NavigationService navigation = NavigationService;

            navigation.Navigate(verificationPage);

            bool verified = await verificationPage.WaitForResultAsync();

            if (navigation.CanGoBack)
            {
                navigation.GoBack();
            }

            return verified;
        }
    }
}
