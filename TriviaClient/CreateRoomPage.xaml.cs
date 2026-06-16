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
        private string username;
        public CreateRoomPage(string username)
        {
            InitializeComponent();
            this.username = username;
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
            string questionCountText = QuestionsCountTextBox.Text;

            if (roomName == "" || questionTimeText == "" || maxPlayersText == "" || questionCountText == "")
            {
                MessageBox.Show("Please fill all fields");
                return;
            }

            int questionTime;
            int maxPlayers;
            int questionCount;

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

            if (!int.TryParse(questionCountText, out questionCount))
            {
                MessageBox.Show("Question count must be a number");
                return;
            }

            if (questionTime <= 0 || maxPlayers <= 0 || questionCount <= 0)
            {
                MessageBox.Show("Values must be positive");
                return;
            }

            if (questionCount > 20)
            {
                MessageBox.Show("Question count must be less than 20");
                return;
            }

            CreateRoomRequest request = new CreateRoomRequest
            {
                RoomName = roomName,
                MaxUsers = (uint)maxPlayers,
                QuestionsCount = (uint)questionCount,
                AnswerTimeout = (uint)questionTime
            };

            try
            {
                byte[] requestBuffer = Serializer.serializeCreateRoomRequest(request);

                CreateRoomResponse response = ClientSession.Communicator.SendEncryptedRequest<CreateRoomResponse>(requestBuffer);

                uint id = response.RoomId;

                if (response.Status == 1)
                {
                    this.NavigationService.Navigate(new WaitingPage(id, roomName, this.username, request.AnswerTimeout, request.QuestionsCount, isAdmin: true));
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
