using System;
using System.Collections.Generic;
using System.Linq;
using System.Printing.IndexedProperties;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Threading;
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
        private Thread t;
        private volatile bool stopThread;
        public JoinRoomPage()
        {
            InitializeComponent();
            this.stopThread = false;

            this.Unloaded += JoinRoomPage_Unloaded;

            // Start the polling thread
            ThreadStart ts = new ThreadStart(FetchRoomsLoop);
            this.t = new Thread(ts);
            t.IsBackground = true;
            t.Start();
        }

        private void JoinRoomPage_Unloaded(object sender, RoutedEventArgs e)
        {
            this.stopThread = true;
        }

        private void FetchRoomsLoop()
        {
            while(!this.stopThread)
            {
                try
                {
                    byte[] requestBuffer = Serializer.SerializeGetRoomsRequest();

                    GetRoomsResponse response = ClientSession.Communicator.SendRequest<GetRoomsResponse>(requestBuffer);

                    if (response.Status == 1)
                    {
                        this.Dispatcher.Invoke(() => UpdateUiWithRooms(response.Rooms));
                    }
                    else
                    {

                        this.Dispatcher.Invoke(() => MessageBox.Show("Failed to load rooms."));
                    }
                }
                catch (Exception ex)
                {
                    if (!stopThread)
                    {
                        this.Dispatcher.Invoke(() => MessageBox.Show("Server error: " + ex.Message));
                    }
                }

                Thread.Sleep(3000);  // wait 3 seconds
            }
        }

        private void UpdateUiWithRooms(List<RoomData> rooms)
        {
            RoomsListBox.Items.Clear();
            RoomsListBox.DisplayMemberPath = "Name";

            int i = 0;
            for (i = 0; i < rooms.Count; i++)
            {
                RoomsListBox.Items.Add(rooms[i]);
            }
        }
        private void GoBack_Click(object sender, RoutedEventArgs e)
        {
            this.stopThread = true;

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
                    byte[] getRoomStateRequestBuffer = { (byte)RequestCode.GetRoomStateRequestCode  };

                    GetRoomStateResponse getRoomState = ClientSession.Communicator.SendRequest<GetRoomStateResponse>(getRoomStateRequestBuffer);

                    this.stopThread = true;
                    this.NavigationService.Navigate(new WaitingPage(request.RoomId, isAdmin: false));
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