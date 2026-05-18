using System;
using System.Collections.Generic;
using System.Linq;
using System.Printing.IndexedProperties;
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
    public partial class JoinRoomPage : Page
    {
        public JoinRoomPage()
        {
            InitializeComponent();
            LoadRooms();
        }

        private void LoadRooms()
        {
            RoomsListBox.Items.Clear();

            try
            {
                byte[] requestBuffer = Serializer.SerializeGetRoomsRequest();

                GetRoomsResponse response = ClientSession.Communicator.SendRequest<GetRoomsResponse>(requestBuffer);

                if (response.Status != 1)
                {
                    MessageBox.Show("Failed to load rooms.");
                    return;
                }

                RoomsListBox.DisplayMemberPath = "Name";

                int i = 0;
                for (i = 0; i < response.Rooms.Count; i++)
                {
                    RoomsListBox.Items.Add(response.Rooms[i]);
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Server error: " + ex.Message);
            }
        }

        private void GoBack_Click(object sender, RoutedEventArgs e)
        {
            if (this.NavigationService.CanGoBack)
            {
                this.NavigationService.GoBack();
            }
        }


        private void JoinRoom_Click(object sender, RoutedEventArgs e)
        {
            if (RoomsListBox.SelectedItem == null)
            {
                MessageBox.Show("Please select a room");
                return;
            }

            RoomData selectedRoom = (RoomData)RoomsListBox.SelectedItem;

            JoinRoomRequest request = new JoinRoomRequest
            {
                RoomId = selectedRoom.Id
            };

            try
            {
                byte[] requestBuffer = Serializer.serializeJoinRoomRequest(request);

                JoinRoomResponse response = ClientSession.Communicator.SendRequest<JoinRoomResponse>(requestBuffer);

                if (response.Status == 1)
                {
                    MessageBox.Show("Joined room successfully.");
                    // later: navigate to waiting room

                    byte[] getRoomStateRequestBuffer = { (byte)RequestCode.GetRoomStateRequestCode  };

                    GetRoomStateResponse getRoomState = ClientSession.Communicator.SendRequest<GetRoomStateResponse>(getRoomStateRequestBuffer);

                    this.NavigationService.Navigate(new WaitingPage(request.RoomId, isAdmin: false));
                    MessageBox.Show("Status: " + getRoomState.Status + "\nRoom Status: " + getRoomState.roomStatus + "\nTime per Question: " + getRoomState.TimePerQuestion + "\nNum of questions per game: " + getRoomState.NumOfQuestionsInGame + "\nPlayers: " + getRoomState.PlayerListRaw);
                }
                else
                {
                    MessageBox.Show("Failed to join room.");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Server error: " + ex.Message);
            }
        }
    }
}