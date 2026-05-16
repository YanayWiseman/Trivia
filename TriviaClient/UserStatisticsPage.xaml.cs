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

                UserStatsListBox.Items.Add("Your name: " + stats[0]);
                UserStatsListBox.Items.Add("Average Answer Time: " + FixNegativeStat(stats[1]));
                UserStatsListBox.Items.Add("Num of Correct Answers: " + FixNegativeStat(stats[2]));
                UserStatsListBox.Items.Add("Games played: " + FixNegativeStat(stats[3]));
                UserStatsListBox.Items.Add("Total Answers: " + FixNegativeStat(stats[4]));

            }
            catch (Exception ex)
            {
                MessageBox.Show("Server error: " + ex.Message);
            }
        }
        private string FixNegativeStat(string value)
        {
            if (int.TryParse(value, out int number))
            {
                return number < 0 ? "0" : value;
            }

            return "0";
        }

        private void GoBack_Click(object sender, RoutedEventArgs e)
        {
            this.NavigationService.Navigate(new StatisticsPage());
        }
    }
}
