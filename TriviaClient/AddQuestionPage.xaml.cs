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
    public partial class AddQuestionPage : Page
    {
        private uint roomId;
        private string username;
        private uint timePerQuestion;
        private uint questionCount;
        const int MAX_ANSWER_ID = 4;

        public AddQuestionPage(uint roomId, string username, uint timePerQuestion, uint questionCount)
        {
            this.roomId = roomId;
            this.username = username;
            this.timePerQuestion = timePerQuestion;
            this.questionCount = questionCount;

            InitializeComponent();
        }

        private void GoBack_Click(object sender, RoutedEventArgs e)
        {
            this.NavigationService.Navigate(new WaitingPage(this.roomId, this.username, this.timePerQuestion, this.questionCount, true));
        }
        private void Submit_Click(object sender, RoutedEventArgs e)
        {
            // input validation
            int correctAnswerId = 0;
            string answer1 = Answer1Box.Text, answer2 = Answer2Box.Text, answer3 = Answer3Box.Text, answer4 = Answer4Box.Text;
            if (answer1 == "" || answer2 == "" || answer3 == "" || answer4 == "")
            {
                MessageBox.Show("Please fill all fields");
                return;
            }
            if (!int.TryParse(CorrectIndexBox.Text, out correctAnswerId))
            {
                MessageBox.Show("Answer id must be a number");
                return;

            }
            if (correctAnswerId <= 0 || correctAnswerId > MAX_ANSWER_ID)
            {
                MessageBox.Show("Answer id must between 1 and 4 and you choose " + correctAnswerId);
                return;
            }

            // craft the add questions request
            correctAnswerId = correctAnswerId - 1;   // it's 0-based in the server

            List<string> userAnswers = new List<string>();
            userAnswers.Add(answer1);
            userAnswers.Add(answer2);
            userAnswers.Add(answer3);
            userAnswers.Add(answer4);

            string question = QuestionBox.Text;

            AddQuestionRequest addQuestioRequest = new AddQuestionRequest
            {
                Question = question,
                Answers = userAnswers,
                CorrectAnswerId = correctAnswerId
            };
            byte[] addQuestionRequestBuffer = Serializer.SerializeAddQuestionRequest(addQuestioRequest);
            AddQuestionResponse addQuestionResponse = ClientSession.Communicator.SendRequest<AddQuestionResponse>(addQuestionRequestBuffer);
            if(addQuestionResponse.Status == 0)
            {
                MessageBox.Show("Failed adding question!");
            }

            this.NavigationService.Navigate(new WaitingPage(this.roomId, this.username, this.timePerQuestion, this.questionCount + 1, true));
        }
    }
}
