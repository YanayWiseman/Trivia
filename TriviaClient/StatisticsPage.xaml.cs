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
    public partial class StatisticsPage : Page
    {
        public StatisticsPage()
        {
            InitializeComponent();
        }
        private void GoToMenu_Click(object sender, RoutedEventArgs e)
        {
            this.NavigationService.Navigate(new MenuPage());
        }

        private void MyStatistics_Click(object sender, RoutedEventArgs e)
        {
            this.NavigationService.Navigate(new UserStatisticsPage());
        }
        private void BestPlayers_Click(object sender, RoutedEventArgs e)
        {
            this.NavigationService.Navigate(new BestPlayersPage());
        }
    }
}
