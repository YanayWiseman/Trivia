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
using System.Windows.Shapes;
using System.Windows.Threading;

namespace TriviaClient
{
    /// <summary>
    /// Interaction logic for OneVsOneCountdownPage.xaml
    /// </summary>
    public partial class OneVsOneCountdownPage : Page
    {
        private string username;
        private uint secondsLeft;
        private uint questionCount;
        private uint timePerQuestion;

        private DispatcherTimer timer;


        /*
            Function constructs the 1v1 countdown page.
            Input: username (string), opponentUsername (string), secondsUntilStart (uint), questionCount (uint), timePerQuestion (uint)
            Output: None (constructor)
        */

        public OneVsOneCountdownPage(string username, string opponentUsername, uint secondsUntilStart, uint questionCount, uint timePerQuestion)
        {
            InitializeComponent();

            this.username = username;
            this.secondsLeft = secondsUntilStart;
            this.questionCount = questionCount;
            this.timePerQuestion = timePerQuestion;

            opponentTextBlock.Text = "Opponent: " + opponentUsername;
            countdownTextBlock.Text = this.secondsLeft.ToString();

            this.timer = new DispatcherTimer();
            this.timer.Interval = TimeSpan.FromSeconds(1);
            this.timer.Tick += TimerTick;
            this.timer.Start();
        }


        /*
            Function updates the countdown timer and navigates to game page when it reaches zero.
            Input: sender (object), e (EventArgs)
            Output: None (void)
        */

        private void TimerTick(object? sender, EventArgs e)
        {
            if (this.secondsLeft > 0)
            {
                this.secondsLeft--;
                countdownTextBlock.Text = this.secondsLeft.ToString();
                return;
            }

            this.timer.Stop();

            this.NavigationService.Navigate(
                new GamePage(this.questionCount, this.timePerQuestion, this.username, true)  // cant leave game
            );
        }
    }
}
