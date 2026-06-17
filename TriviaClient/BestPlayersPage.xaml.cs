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
    public partial class BestPlayersPage : Page
    {
        public BestPlayersPage()
        {
            InitializeComponent();
            LoadBestPlayers();
        }

        /*
            Function Loads best players statistics from server.
            Input: None
            Output: None
        */

        private void LoadBestPlayers()
        {
            try
            {
                byte[] requestBuffer = Serializer.SerializeHighScoreRequest();

                GetHighScoreResponse response = ClientSession.Communicator.SendEncryptedRequest<GetHighScoreResponse>(requestBuffer);

                if (response.Status != 1)
                {
                    MessageBox.Show("Failed to load best players.");
                    return;
                }

                List<string> highScores = response.Statistics;

                BestPlayersListBox.Items.Clear();

                int i = 0;
                for (i = 0; i < highScores.Count; i++)
                {
                    BestPlayersListBox.Items.Add(highScores[i]);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Server error: " + ex.Message);
            }
        }

        /*
            Function Goes back to statistics page.
            Input: sender (object), e (RoutedEventArgs)
            Output: None
        */

        private void GoBack_Click(object sender, RoutedEventArgs e)
        {
            this.NavigationService.GoBack();
        }
    }
}
