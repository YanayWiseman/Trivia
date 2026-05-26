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
    public partial class GameResultsPage : Page
    {
        public List<PlayerResult> leaderboard { get; set; }
        public GameResultsPage()
        {
            InitializeComponent();

            this.leaderboard = FetchGameResults();

            // tell the xaml to look at this class for data bindings
            this.DataContext = this;
        }

        private List<PlayerResult> FetchGameResults()
        {
            return new List<PlayerResult>
            {
                new PlayerResult { Rank = 1, PlayerName = "Kira", CorrectAnswers = 10 },
                new PlayerResult { Rank = 2, PlayerName = "Yanay", CorrectAnswers = 6 },
                new PlayerResult { Rank = 3, PlayerName = "Radinsky", CorrectAnswers = 4 },
                new PlayerResult { Rank = 4, PlayerName = "Alan", CorrectAnswers = 2 }
            };
        }
        public class PlayerResult
        {
            public int Rank { get; set; }
            public string? PlayerName { get; set; }
            public int CorrectAnswers { get; set; }
        }
    }
}
