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
    /// <summary>
    /// Interaction logic for MenuPage.xaml
    /// </summary>
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
        }
        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            // log out first !!!!!!!!! WE NEED TO ADD THIS
            Application.Current.Shutdown();
        }
    }
}
