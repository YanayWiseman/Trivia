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
    public partial class CreateRoomPage : Page
    {
        public CreateRoomPage()
        {
            InitializeComponent();
        }

        private void GoBack_Click(object sender, RoutedEventArgs e)
        {
            NavigationService.GoBack();
        }

        private void CreateRoom_Click(object sender, RoutedEventArgs e)
        {
            string roomName = RoomNameTextBox.Text;
            string questionTimeText = QuestionTimeTextBox.Text;
            string maxPlayersText = MaxPlayersTextBox.Text;

            if (roomName == "" || questionTimeText == "" || maxPlayersText == "")
            {
                MessageBox.Show("Please fill all fields");
                return;
            }

            int questionTime;
            int maxPlayers;

            if (!int.TryParse(questionTimeText, out questionTime))
            {
                MessageBox.Show("Question time must be a number");
                return;
            }

            if (!int.TryParse(maxPlayersText, out maxPlayers))
            {
                MessageBox.Show("Max players must be a number");
                return;
            }

            if (questionTime <= 0 || maxPlayers <= 0)
            {
                MessageBox.Show("Values must be positive.");
                return;
            }

            CreateRoomRequest request = new CreateRoomRequest
            {
                RoomName = roomName,
                MaxUsers = (uint)maxPlayers,
                QuestionsCount = 10,
                AnswerTimeout = (uint)questionTime
            };

            try
            {
                byte[] requestBuffer = Serializer.serializeCreateRoomRequest(request);

                CreateRoomResponse response = ClientSession.Communicator.SendRequest<CreateRoomResponse>(requestBuffer);

                uint id = response.RoomId;

                if (response.Status == 1)
                {
                    MessageBox.Show("Room created successfully.");
                    this.NavigationService.Navigate(new WaitingPage(id, request.AnswerTimeout, request.QuestionsCount, isAdmin: true));
                }
                else
                {
                    MessageBox.Show("Failed to create room.");
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Server error: " + ex.Message);
            }
        }
    }
}
