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
using System.ComponentModel;

namespace TriviaClient
{
    public partial class MenuPage : Page
    {
        public MenuPage(string username)
        {
            InitializeComponent();

            userNameTextBlock.Text = username;
        }

        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            this.NavigationService.Navigate(new CreateRoomPage(userNameTextBlock.Text));
        }
        private void JoinRoom_Click(object sender, RoutedEventArgs e)
        {
            this.NavigationService.Navigate(new JoinRoomPage(userNameTextBlock.Text));
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
                ClientSession.Communicator.SendRequest<LogoutResponse>(requestBuffer);
                ClientSession.Communicator.Disconnect();
                ClientSession.Communicator = new Communicator();
                this.NavigationService.Navigate(new MainPage());
            }
            catch (Exception ex)
            {
                MessageBox.Show("Server error: " + ex.Message);
            }
        }
        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();  // MainWindow.Closing handles the rest
        }
    }
}
