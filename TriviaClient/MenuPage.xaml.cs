using Newtonsoft.Json;
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
using System.ComponentModel;

namespace TriviaClient
{
    public partial class MenuPage : Page
    {
        public MenuPage(string username)
        {
            InitializeComponent();

            userNameTextBlock.Text = username;
        }

        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            this.NavigationService.Navigate(new CreateRoomPage(userNameTextBlock.Text));
        }
        private void JoinRoom_Click(object sender, RoutedEventArgs e)
        {
            this.NavigationService.Navigate(new JoinRoomPage(userNameTextBlock.Text));
        }
        private void Statistics_Click(object sender, RoutedEventArgs e)
        {
            this.NavigationService.Navigate(new StatisticsPage());
        }
        private void Logout_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                byte[] requestBuffer = Serializer.SerializeLogoutRequest();
                ClientSession.Communicator.SendEncryptedRequest<LogoutResponse>(requestBuffer);
                ClientSession.Communicator.Disconnect();
                ClientSession.Communicator = new Communicator();
                this.NavigationService.Navigate(new MainPage());
            }
            catch (Exception ex)
            {
                MessageBox.Show("Server error: " + ex.Message);
            }
        }
        private void Exit_Click(object sender, RoutedEventArgs e)
        {
            Application.Current.Shutdown();  // MainWindow.Closing handles the rest
        }

        /*
    Function sends a 1v1 queue request and navigates according to the response.
    Input: sender (object), e (RoutedEventArgs)
    Output: None (void)
*/

        private void OneVsOne_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                byte[] requestBuffer = Serializer.SerializeJoinOneVsOneQueueRequest();

                OneVsOneStatusResponse response =ClientSession.Communicator.SendEncryptedRequest<OneVsOneStatusResponse>(requestBuffer);

                if (response.Status != 1)
                {
                    MessageBox.Show("Failed to join 1v1 queue.");
                    return;
                }

                if (response.QueueStatus == (uint)OneVsOneQueueStatus.OneVsOneWaiting)
                {
                    this.NavigationService.Navigate(new OneVsOneQueuePage(userNameTextBlock.Text));
                }
                else if (response.QueueStatus == (uint)OneVsOneQueueStatus.OneVsOneMatched)
                {
                    this.NavigationService.Navigate(
                        new OneVsOneCountdownPage(
                            userNameTextBlock.Text,
                            response.OpponentUsername,
                            response.SecondsUntilStart,
                            response.QuestionsCount,
                            response.AnswerTimeout
                        )
                    );
                }
                else
                {
                    MessageBox.Show("Unknown 1v1 queue status.");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Server error: " + ex.Message);
            }
        }
    }
}


