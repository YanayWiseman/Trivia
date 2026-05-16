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
    public partial class UserStatisticsPage : Page
    {
        public UserStatisticsPage()
        {
            InitializeComponent();
            LoadUserStats();
        }

        private void LoadUserStats()
        {
            try
            {
                byte[] requestBuffer = Serializer.SerializeGetPersonalStatsRequest();

                GetPersonalStatsResponse response = ClientSession.Communicator.SendRequest<GetPersonalStatsResponse>(requestBuffer);

                if (response.Status != 1)
                {
                    MessageBox.Show("Failed to load personal statistics.");
                    return;
                }

                List<string> stats = response.GetStats();

                int i = 0;
                for (i = 0; i < stats.Count; i++)
                {
                    UserStatsListBox.Items.Add(stats[i]);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Server error: " + ex.Message);
            }
        }

        private void GoBack_Click(object sender, RoutedEventArgs e)
        {
            this.NavigationService.Navigate(new StatisticsPage());
        }
    }
}
