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
    public partial class WaitingPage : Page
    {
        private uint roomId;
        private bool isAdmin;
        private Thread t;
        private volatile bool stopThread;
        private uint timePerQuestion;
        private uint questionCount;
        private string username;

        public WaitingPage(uint roomId, string username, uint timePerQuestion = 0, uint questionCount = 0, bool isAdmin = false)
        {
            this.roomId = roomId;
            this.isAdmin = isAdmin;
            this.username = username;
            if (isAdmin)
            {
                this.timePerQuestion = timePerQuestion;
                this.questionCount = questionCount;
            }

            InitializeComponent();

            this.Unloaded += WaitingPage_Unloaded;

            if (!this.isAdmin)
            {
                CloseRoomButton.IsEnabled = false;
                CloseRoomButton.Background = new SolidColorBrush(Colors.Gray);
                StartGameButton.IsEnabled = false;
                StartGameButton.Background = new SolidColorBrush(Colors.Gray);
            }
            this.stopThread = false;

            // Start the polling thread
            ThreadStart ts = new ThreadStart(FetchPlayersLoop);
            this.t = new Thread(ts);
            t.IsBackground = true;
            t.Start();
        }

        private void WaitingPage_Unloaded(object sender, RoutedEventArgs e)
        {
            this.stopThread = true;
        }

        private void FetchPlayersLoop()
        {
            while (!this.stopThread)
            {
                try
                {
                    byte[] getRoomStateRequestBuffer = { (byte)RequestCode.GetRoomStateRequestCode };

                    GetRoomStateResponse getRoomState = ClientSession.Communicator.SendRequest<GetRoomStateResponse>(getRoomStateRequestBuffer);

                    if (getRoomState.Status == 0 || getRoomState.roomStatus == RoomStatus.Closed)
                    {
                        this.stopThread = true;

                        this.Dispatcher.Invoke(() =>
                        {
                            MessageBox.Show("Room has been closed.");
                            this.NavigationService.Navigate(new JoinRoomPage(this.username));
                        });

                        break;
                    }
                    if (getRoomState.roomStatus == RoomStatus.InGame)
                    {
                        this.stopThread = true;
                        this.Dispatcher.Invoke(() =>
                        {
                            this.NavigationService.Navigate(new GamePage(this.questionCount, this.timePerQuestion, this.username));
                        });
                    }

                    GetPlayersInRoomRequest request = new GetPlayersInRoomRequest();
                    request.RoomId = this.roomId;

                    byte[] requestBuffer = Serializer.serializeGetPlayersRequest(request);

                    GetPlayersInRoomResponse response = ClientSession.Communicator.SendRequest<GetPlayersInRoomResponse>(requestBuffer);

                    if (response.Status == 1)
                    {
                        this.Dispatcher.Invoke(() => UpdateUiWithPlayers(response.GetPlayers()));
                    }
                    else
                    {

                        this.Dispatcher.Invoke(() => MessageBox.Show("Failed to load players."));
                    }
                }
                catch (Exception ex)
                {
                    if (!stopThread)
                    {
                        this.Dispatcher.Invoke(() => MessageBox.Show("Server error: " + ex.Message));
                    }
                }
                Thread.Sleep(3000);
            }
        }
        private void UpdateUiWithPlayers(List<string> players)
        {
            PlayerBox.Items.Clear();

            int i = 0;
            for (i = 0; i < players.Count; i++)
            {
                PlayerBox.Items.Add(players[i]);
            }
        }

        private void LeaveRoom_Click(object sender, RoutedEventArgs e)
        {
            byte[] requestBuffer = { (byte)RequestCode.LeaveRoomRequestCode };

            LeaveRoomResponse response = ClientSession.Communicator.SendRequest<LeaveRoomResponse>(requestBuffer);
            if (response.Status == 1)
            {
                this.stopThread = true;
                this.NavigationService.Navigate(new JoinRoomPage(this.username));
            }
            else
            {
                MessageBox.Show("Leaving failed, you must stay forever!!!!!");
            }
        }

        private void StartGame_Click(object sender, RoutedEventArgs e)
        {
            byte[] requestBuffer = Serializer.SerializeStartGameRequest();

            StartGameResponse response = ClientSession.Communicator.SendRequest<StartGameResponse>(requestBuffer);
            if (response.Status == 1)
            {
                this.stopThread = true;
                // temp
                if (this.NavigationService != null)
                {
                    this.NavigationService.Navigate(new GamePage(this.questionCount, this.timePerQuestion, this.username));
                }
                else
                {
                    MessageBox.Show("NavigationService is null!");
                }
            }
            else
            {
                MessageBox.Show("Failed starting game!");
            }
        }

        private void CloseRoom_Click(object sender, RoutedEventArgs e)
        {
            this.stopThread = true;

            byte[] requestBuffer = { (byte)RequestCode.CloseRoomRequestCode };

            CloseRoomResponse response = ClientSession.Communicator.SendRequest<CloseRoomResponse>(requestBuffer);

            if (response.Status == 1)
            {
                this.NavigationService.Navigate(new JoinRoomPage(this.username));

                while (this.NavigationService.CanGoBack)
                {
                    this.NavigationService.RemoveBackEntry(); 
                }
            }
            else
            {
                MessageBox.Show("Only the admin can close the room.");
            }
        }

        // called by MainWindow when X is clicked while on this page
        public void CleanupOnClose()
        {
            if (isAdmin)
            {
                byte[] requestBuffer = { (byte)RequestCode.CloseRoomRequestCode };
                ClientSession.Communicator.SendRequest<CloseRoomResponse>(requestBuffer);
            }
            else
            {
                byte[] requestBuffer = { (byte)RequestCode.LeaveRoomRequestCode };
                ClientSession.Communicator.SendRequest<LeaveRoomResponse>(requestBuffer);
            }
        }

    }
}
