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
    /// Interaction logic for OneVsOneQueuePage.xaml
    /// </summary>
    public partial class OneVsOneQueuePage : Page
    {
        private string username;
        private bool stopPinging;


        /*
            Function constructs the 1v1 queue page.
            Input: username (string)
            Output: None (constructor)
        */

        public OneVsOneQueuePage(string username)
        {
            InitializeComponent();

            this.username = username;
            this.stopPinging = false;

            StartPinging();
        }


        /*
            Function pings the server until a 1v1 match is found.
            Input: None
            Output: None (void)
        */

        private async void StartPinging()
        {
            while (!this.stopPinging)
            {
                try
                {
                    OneVsOneStatusResponse response = await Task.Run(() =>
                    {
                        byte[] requestBuffer = Serializer.SerializeGetOneVsOneStatusRequest();
                        return ClientSession.Communicator.SendRequest<OneVsOneStatusResponse>(requestBuffer);
                    });

                    if (response.Status != 1)
                    {
                        statusTextBlock.Text = "Failed to search for match.";
                    }
                    else if (response.QueueStatus == (uint)OneVsOneQueueStatus.OneVsOneMatched)
                    {
                        this.stopPinging = true;

                        this.NavigationService.Navigate(
                            new OneVsOneCountdownPage(this.username, response.OpponentUsername, response.SecondsUntilStart, response.QuestionsCount, response.AnswerTimeout)
                        );

                        return;
                    }
                    else if (response.QueueStatus == (uint)OneVsOneQueueStatus.OneVsOneWaiting)
                    {
                        statusTextBlock.Text = "Waiting for opponent...";
                    }
                    else
                    {
                        statusTextBlock.Text = "You are not in the queue.";
                    }
                }
                catch (Exception ex)
                {
                    statusTextBlock.Text = "Server error: " + ex.Message;
                }

                await Task.Delay(1000);
            }
        }

        /*
            Function cancels the 1v1 queue and returns to the menu.
            Input: sender (object), e (RoutedEventArgs)
            Output: None (void)
        */

        private void Cancel_Click(object sender, RoutedEventArgs e)
        {
            this.stopPinging = true;

            try
            {
                byte[] requestBuffer = Serializer.SerializeCancelOneVsOneQueueRequest();
                ClientSession.Communicator.SendRequest<OneVsOneStatusResponse>(requestBuffer);
            }
            catch
            {
            }

            this.NavigationService.Navigate(new MenuPage(this.username));
        }
    }
}
