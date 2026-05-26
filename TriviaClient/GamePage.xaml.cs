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
    /// Interaction logic for GamePage.xaml
    /// </summary>
    public partial class GamePage : Page
    {
        private uint timeLeft;
        private uint questionNumber;
        private uint questionCount;
        private uint timePerQuestion;

        private bool answered;
        private bool waitingForAnswerResult;

        private SubmitAnswerResponse? lastSubmitResponse;
        private Button? clickedButton;

        public GamePage()
        {
            InitializeComponent();
        }

        private async void AnswerButton_Click(object sender, RoutedEventArgs e)
        {
            if (this.answered)
            {
                return;
            }

            this.answered = true;
            this.waitingForAnswerResult = true;

            this.clickedButton = (Button)sender;
            uint answerId = Convert.ToUInt32(this.clickedButton.Tag);

            // disable button here

            try
            {
                SubmitAnswerRequest request = new SubmitAnswerRequest
                {
                    AnswerId = answerId
                };

                this.lastSubmitResponse = await Task.Run(() =>
                {
                    byte[] requestBuffer = Serializer.SerializeSubmitAnswerRequest(request);

                    return ClientSession.Communicator.SendRequest<SubmitAnswerResponse>(requestBuffer);
                });

                this.waitingForAnswerResult = false;

                if (this.timeLeft == 0)
                {
                    // show results
                }
            }
            catch (Exception ex)
            {
                MessageBox.Show("Server error: " + ex.Message);
            }
        }
    }
}
