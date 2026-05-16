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
        public MenuPage()
        {
            InitializeComponent();

            Loaded += MenuPage_Loaded;
        }

        private void MenuPage_Loaded(object sender, RoutedEventArgs e)
        {
            Window parentWindow = Window.GetWindow(this);

            if (parentWindow != null)
            {
                parentWindow.Closing -= ParentWindow_Closing;
                parentWindow.Closing += ParentWindow_Closing;
            }
        }
        private void ParentWindow_Closing(object? sender, CancelEventArgs e)
        {
            PerformLogout();
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
        private void PerformLogout()
        {
            try
            {
                byte[] requestBuffer = Serializer.SerializeLogoutRequest();

                ClientSession.Communicator.SendRequest<LogoutResponse>(requestBuffer);
            }
            catch
            {
            }

            try
            {
                ClientSession.Communicator.Disconnect();
            }
            catch
            {
            }
        }
        private void Logout_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                PerformLogout();

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
            PerformLogout();
            Application.Current.Shutdown();
        }
    }
}
