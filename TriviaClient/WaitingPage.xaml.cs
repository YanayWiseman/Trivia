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
        public WaitingPage(uint roomId, bool isAdmin = false)
        {
            this.roomId = roomId;
            this.isAdmin = isAdmin;

            InitializeComponent();

            if(!this.isAdmin)
            {
                CloseRoomButton.IsEnabled = false;
                CloseRoomButton.Background = new SolidColorBrush(Colors.Gray);
            }
            LoadPlayers();
        }
        private void LoadPlayers()
        {
            PlayerBox.Items.Clear();

            try
            {
                GetPlayersInRoomRequest request = new GetPlayersInRoomRequest();
                request.RoomId = this.roomId;

                byte[] requestBuffer = Serializer.serializeGetPlayersRequest(request);

                GetPlayersInRoomResponse response = ClientSession.Communicator.SendRequest<GetPlayersInRoomResponse>(requestBuffer);

                if (response.Status != 1)
                {
                    MessageBox.Show("Failed to load players.");
                    return;
                }

                List<string> usernames = response.GetPlayers();

                PlayerBox.Items.Clear();

                int i = 0;
                for (i = 0; i < usernames.Count; i++)
                {
                    PlayerBox.Items.Add(usernames[i]);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Server error: " + ex.Message);
            }
        }
        private void LeaveRoom_Click(object sender, RoutedEventArgs e)
        {
            byte[] requestBuffer = { (byte)RequestCode.LeaveRoomRequestCode };

            LeaveRoomResponse response = ClientSession.Communicator.SendRequest<LeaveRoomResponse>(requestBuffer);
            if (response.Status == 1)
            {
                this.NavigationService.GoBack();
            }
            else
            {
                MessageBox.Show("Leaving failed, you must stay forever!!!!!");
            }
        }

        private void StartGame_Click(object sender, RoutedEventArgs e)
        {
            byte[] requestBuffer = { (byte)RequestCode.StartGameRequestCode };

            StartGameResponse response = ClientSession.Communicator.SendRequest<StartGameResponse>(requestBuffer);
            if (response.Status == 1)
            {
                this.NavigationService.GoBack();
            }
            else
            {
                MessageBox.Show("Failed starting game!");
            }
        }

        private void CloseRoom_Click(object sender, RoutedEventArgs e)
        {
            byte[] requestBuffer = { (byte)RequestCode.CloseRoomRequestCode };

            CloseRoomResponse response = ClientSession.Communicator.SendRequest<CloseRoomResponse>(requestBuffer);

            if (response.Status == 1)
            {
                this.NavigationService.GoBack();
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
