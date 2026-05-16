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

        private void GoToMenu_Click(object sender, RoutedEventArgs e)
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
                    this.NavigationService.Navigate(new MenuPage());
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
    }
}
