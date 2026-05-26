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
        public GameResultsPage()
        {
            InitializeComponent();
            byte[] getGameResultsRequestBuffer = { (byte)RequestCode.GetGameResultsRequestCode };
            GetGameResultResponse getGameResultsResponse = ClientSession.Communicator.SendRequest<GetGameResultResponse>(getGameResultsRequestBuffer);

            if(getGameResultsResponse.Status == 1)
            {
                this.leaderboard = FetchGameResults(getGameResultsResponse.results);
            }

            // tell the xaml to look at this class for data bindings
            this.DataContext = this;
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
    }
}
