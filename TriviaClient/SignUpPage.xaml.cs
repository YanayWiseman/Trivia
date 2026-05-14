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
using TriviaClient;

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
            try
            {
                // get input from the clearableTextBox's
                // and craft it into a SignUpRequest
                SignupRequest request = new SignupRequest
                {
                    Username = usernameBox.Text,
                    Password = passwordBox.Text,
                    Email = mailBox.Text,
                    Address = addressBox.Text,
                    Phone = phoneBox.Text,

                    // this is how to access the picked date through the birthDatePicket.
                    // question mark means it can be null, ToString casts it to a string in the given Format
                    // ?? means its a terinary condition
                    Birthdate = BirthDatePicker.SelectedDate?.ToString("dd/MM/yyyy") ?? ""
                };

                // now we can serialize the request and send it
                byte[] requestBytes = Serializer.serializeSignupRequest(request);

                SignupResponse response = AppState.Communicator.SendRequest<SignupResponse>(requestBytes);

                // move to the menu
                if(response.Status == 1)
                {
                    this.NavigationService.Navigate(new MainPage());
                }
                else
                {
                    MessageBox.Show("Signup failed!\nOne of the fields are not in the correct format!");
                }
            }
            catch (Exception exepction)
            {
                MessageBox.Show("Error: " + exepction.Message);
            }
        }
    }
}
