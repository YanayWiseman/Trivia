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
using System.Windows.Threading;

namespace TriviaClient
{

    public partial class GamePage : Page
    {
        private DispatcherTimer timer;

        private uint timeLeft;
        private uint questionNumber;
        private uint questionCount;
        private uint timePerQuestion;

        private bool answered;
        private bool waitingForAnswerResult;

        private SubmitAnswerResponse? lastSubmitResponse;
        private Button? clickedButton;

        private List<Button> answerButtons;
        private List<TextBlock> answerTextBlocks;
        private string username;


        /*
            Function constructs a GamePage object
            Input: questionCount (uint), timePerQuestion (uint)
            Output: None (constructor)
        */

        public GamePage(uint questionCount, uint timePerQuestion, string username)
        {
            InitializeComponent();

            this.questionCount = questionCount;
            this.timePerQuestion = timePerQuestion;
            this.questionNumber = 0;
            this.username = username;

            this.answered = false;
            this.waitingForAnswerResult = false;

            this.answerButtons = new List<Button>
            {
                answerButton1,
                answerButton2,
                answerButton3,
                answerButton4
            };

           this.answerTextBlocks = new List<TextBlock>
           {
                answerTextBlock1,
                answerTextBlock2,
                answerTextBlock3,
                answerTextBlock4
           };

            this.timer = new DispatcherTimer();
            this.timer.Interval = TimeSpan.FromSeconds(1);
            this.timer.Tick += TimerTick;

            LoadNextQuestion();
        }


        /*
            Function displays the answer options on the answer buttons
            Input: answers (Dictionary<uint, string>)
            Output: None (void)
        */

        private void DisplayAnswers(Dictionary<uint, string> answers)
        {
            List<KeyValuePair<uint, string>> answerList = answers.ToList();

            for (int i = 0; i < this.answerButtons.Count; i++)
            {
                if (i < answerList.Count)
                {
                    this.answerButtons[i].Tag = answerList[i].Key;
                    this.answerTextBlocks[i].Text = answerList[i].Value;
                    this.answerButtons[i].Visibility = Visibility.Visible;
                }
                else
                {
                    this.answerButtons[i].Visibility = Visibility.Hidden;
                }
            }
        }


        /*
            Function loads the next question from the server and displays it on the game page
            Input: None
            Output: None (void)
        */

        private void LoadNextQuestion()
        {
            this.answered = false;
            this.waitingForAnswerResult = false;
            this.lastSubmitResponse = null;
            this.clickedButton = null;

            ResetButtonColors();
            EnableAnswerButtons();

            byte[] request = Serializer.SerializeGetQuestionRequest();
            GetQuestionResponse response = ClientSession.Communicator.SendRequest<GetQuestionResponse>(request);

            if (response.Status != 1)
            {
                ShowResultsPage();
                return;
            }

            this.questionNumber++;

            questionNumberTextBlock.Text = "Question " + this.questionNumber + " / " + this.questionCount;
            questionTextBlock.Text = response.Question;

            DisplayAnswers(response.Answers);

            this.timeLeft = this.timePerQuestion;
            timerTextBlock.Text = this.timeLeft.ToString();

            this.timer.Stop();
            this.timer.Start();
        }


        /*
            Function updates the game timer and submits an empty answer when time runs out
            Input: sender (object), e (EventArgs)
            Output: None (void)
        */

        private async void TimerTick(object? sender, EventArgs e)
        {
            if (this.timeLeft > 0)
            {
                this.timeLeft--;
                timerTextBlock.Text = this.timeLeft.ToString();
                return;
            }

            this.timer.Stop();

            if (!this.answered)
            {
                this.answered = true;
                this.waitingForAnswerResult = true;

                DisableAnswerButtons();

                SubmitAnswerRequest request = new SubmitAnswerRequest();
                request.AnswerId = 0;

                this.lastSubmitResponse = await Task.Run(() =>
                {
                    byte[] requestBuffer = Serializer.SerializeSubmitAnswerRequest(request);

                    return ClientSession.Communicator.SendRequest<SubmitAnswerResponse>(requestBuffer);
                });

                this.waitingForAnswerResult = false;
            }

            if (!this.waitingForAnswerResult)
            {
                await ShowAnswerResult();
            }
        }


        /*
             Function handles an answer button click and sends the selected answer to the server
             Input: sender (object), e (RoutedEventArgs)
             Output: None (void)
        */

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

            DisableAnswerButtons();

            SubmitAnswerRequest request = new SubmitAnswerRequest();
            request.AnswerId = answerId;

            this.lastSubmitResponse = await Task.Run(() =>
            {
                byte[] requestBuffer = Serializer.SerializeSubmitAnswerRequest(request);

                return ClientSession.Communicator.SendRequest<SubmitAnswerResponse>(requestBuffer);
            });

            this.waitingForAnswerResult = false;
            await ShowAnswerResult();
        }


        /*
            Function displays the answer result after the server returns the correct answer
            Input: None
            Output: None (void)
        */

        private async Task ShowAnswerResult()
        {
            if (this.lastSubmitResponse == null)
            {
                return;
            }

            uint clickedAnswerId = 0;

            if (this.clickedButton != null)
            {
                clickedAnswerId = Convert.ToUInt32(this.clickedButton.Tag);
            }

            MarkAnswer(clickedAnswerId, this.lastSubmitResponse.CorrectAnswerId);

            await Task.Delay(1200);

            if (this.questionNumber >= this.questionCount)
            {
                ShowResultsPage();
            }
            else
            {
                LoadNextQuestion();
            }
        }


        /*
            Function marks the correct answer and the selected wrong answer using different colors
            Input: clickedAnswerId (uint), correctAnswerId (uint)
            Output: None (void)
        */

        private void MarkAnswer(uint clickedAnswerId, uint correctAnswerId)
        {
            for (int i = 0; i < this.answerButtons.Count; i++)
            {
                if (this.answerButtons[i].Tag == null)
                {
                    continue;
                }

                uint buttonAnswerId = Convert.ToUInt32(this.answerButtons[i].Tag);

                if (buttonAnswerId == correctAnswerId)
                {
                    this.answerButtons[i].Background = Brushes.LightGreen;
                }
                else if (buttonAnswerId == clickedAnswerId && clickedAnswerId != 0)
                {
                    this.answerButtons[i].Background = Brushes.IndianRed;
                }
            }
        }


        /*
            Function resets the answer buttons to their original colors
            Input: None
            Output: None (void)
        */

        private void ResetButtonColors()
        {
            answerButton1.Background = (Brush)new BrushConverter().ConvertFrom("#E53935")!;
            answerButton2.Background = (Brush)new BrushConverter().ConvertFrom("#FB8C00")!;
            answerButton3.Background = (Brush)new BrushConverter().ConvertFrom("#FDD835")!;
            answerButton4.Background = (Brush)new BrushConverter().ConvertFrom("#8D2C00")!;
        }


        /*
            Function enables all answer buttons
            Input: None
            Output: None (void)
        */

        private void EnableAnswerButtons()
        {
            for (int i = 0; i < this.answerButtons.Count; i++)
            {
                this.answerButtons[i].IsEnabled = true;
            }
        }


        /*
            Function disables all answer buttons
            Input: None
            Output: None (void)
        */

        private void DisableAnswerButtons()
        {
            for (int i = 0; i < this.answerButtons.Count; i++)
            {
                this.answerButtons[i].IsEnabled = false;
            }
        }


        /*
            Function stops the game timer and moves the user to the results page
            Input: None
            Output: None (void)
        */

        private void ShowResultsPage()
        {
            this.timer.Stop();

            this.NavigationService.Navigate(new GameResultsPage(this.username));
        }
    }
}
