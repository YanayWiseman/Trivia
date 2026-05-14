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

        private void LoginButton_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                // get input from the clearableTextBox's
                // and craft it into a LoginRequest
                LoginRequest request = new LoginRequest
                {
                    Username = UsernameTextBox.Text,
                    Password = PasswordBox.Password
                };

                // now we can serialize the request and send it
                byte[] requestBytes = Serializer.SerializeLoginRequest(request);

                LoginResponse response = AppState.Communicator.SendRequest<LoginResponse>(requestBytes);

                // move to the menu
                if (response.Status == 1)
                {
                    this.NavigationService.Navigate(new MenuPage());
                }
                else
                {
                    MessageBox.Show("Login failed!\nUser doesn't exist!");
                }
            }
            catch (Exception exepction)
            {
                MessageBox.Show("Error: " + exepction.Message);
            }
        }
    }
}
