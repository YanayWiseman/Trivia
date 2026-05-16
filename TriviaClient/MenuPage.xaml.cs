using Newtonsoft.Json;
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
    public partial class MenuPage : Page
    {
        public MenuPage()
        {
            InitializeComponent();
        }
        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            this.NavigationService.Navigate(new CreateRoomPage());
        }
        private void JoinRoom_Click(object sender, RoutedEventArgs e)
        {
            this.NavigationService.Navigate(new JoinRoomPage());
        }
        private void Statistics_Click(object sender, RoutedEventArgs e)
        {
            this.NavigationService.Navigate(new StatisticsPage());
        }
        private void Logout_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                byte[] requestBuffer = Serializer.SerializeLogoutRequest();

                LogoutResponse response = ClientSession.Communicator.SendRequest<LogoutResponse>(requestBuffer);

                if (response.Status == 1)
                {
                    ClientSession.Communicator.Disconnect();
                    ClientSession.Communicator = new Communicator();

                    this.NavigationService.Navigate(new MainPage());
                }
                else
                {
                    MessageBox.Show("Logout failed.");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Server error: " + ex.Message);
            }
        }

        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                byte[] requestBuffer = Serializer.SerializeLogoutRequest();

                ClientSession.Communicator.SendRequest<LogoutResponse>(requestBuffer);
            }
            catch
            {
                // App is closing anyway
            }

            try
            {
                ClientSession.Communicator.Disconnect();
            }
            catch
            {
            }

            Application.Current.Shutdown();
        }
    }
}
