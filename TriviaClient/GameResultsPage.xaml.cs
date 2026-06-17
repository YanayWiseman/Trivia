using System;
using System.Collections;
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
    public partial class GameResultsPage : Page
    {
        public List<PlayerResult>? leaderboard { get; set; }
        private string username;
        private bool stopPolling;

        public GameResultsPage(string username)
        {
            InitializeComponent();
            
            // tell the xaml to look at this class for data bindings
            this.DataContext = this;
            this.username = username;
            Loaded += GameResultsPage_Loaded;
        }

        private async void GameResultsPage_Loaded(object sender, RoutedEventArgs e)
        {
            stopPolling = false;

            while(!this.stopPolling)
            {
                try
                {
                    byte[] getGameResultsRequestBuffer = Serializer.SerializeGetGameResultsRequest();
                    GetGameResultResponse getGameResultsResponse =ClientSession.Communicator.SendEncryptedRequest<GetGameResultResponse>(getGameResultsRequestBuffer);

                    if (getGameResultsResponse.Status == 1)
                    {
                        this.leaderboard = FetchGameResults(getGameResultsResponse.results);
                        ResultsList.ItemsSource = leaderboard;
                        break;
                    }
                    MessageBox.Show("The server says the game has not ended yet.");
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Server error: " + ex.Message);
                }

                await Task.Delay(300);
            }
        }

        public void CleanupOnClose()
        {
            this.stopPolling = true;
                        try
            {
                // tell the server to transition back to MenuRequestHandler
                byte[] leaveBuffer = Serializer.SerializeLeaveGameRequest();
                LeaveGameResponse response = ClientSession.Communicator.SendEncryptedRequest<LeaveGameResponse>(leaveBuffer);

                if (response.Status == 0)
                {
                    MessageBox.Show("Failed to leave the game.");
                }
            }
            catch { }
        }

        private List<PlayerResult>? FetchGameResults(List<ServerPlayerResult>? playersResults)
        {
            if (playersResults == null)
            {
                return new List<PlayerResult>();
            }

            // sort the list by the correct answers count
            List<ServerPlayerResult> sortedPlayers = playersResults.OrderByDescending(p => p.CorrectAnswerCount).ToList();
            List<PlayerResult> rankingList = new List<PlayerResult>();

            int i = 0;
            for (i = 0; i < sortedPlayers.Count; i++)
            {
                ServerPlayerResult curr = sortedPlayers[i];
                rankingList.Add(new PlayerResult
                {
                    Rank = i + 1,
                    CorrectAnswers = curr.CorrectAnswerCount,
                    PlayerName = curr.Username
                });
            }

            return rankingList;
        }
        public class PlayerResult
        {
            public int Rank { get; set; }
            public string? PlayerName { get; set; }
            public int CorrectAnswers { get; set; }
        }

        private void BackToMenu_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                // tell the server to transition back to MenuRequestHandler
                byte[] leaveBuffer = Serializer.SerializeLeaveGameRequest();
                ClientSession.Communicator.SendEncryptedRequest<LeaveGameResponse>(leaveBuffer);
            }
            catch { }
            this.NavigationService.Navigate(new MenuPage(username));
        }
    }
}
