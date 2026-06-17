using System;
using System.ComponentModel;
using System.IO;
using System.Windows;
using System.Windows.Controls;



namespace TriviaClient
{
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            LoadBackgroundVideo();
            this.Closing += MainWindow_Closing;
        }

        private void LoadBackgroundVideo()
        {
            string videoPath = Path.Combine(
                AppDomain.CurrentDomain.BaseDirectory,
                "Assets",
                "background.mp4"
            );

            if (!File.Exists(videoPath))
            {
                MessageBox.Show(
                    "Video not found:\n" + videoPath,
                    "Missing background video",
                    MessageBoxButton.OK,
                    MessageBoxImage.Error
                );

                return;
            }

            BackgroundVideo.Source = new Uri(videoPath, UriKind.Absolute);
            BackgroundVideo.Play();
        }

        private void BackgroundVideo_MediaEnded(object sender, RoutedEventArgs e)
        {
            BackgroundVideo.Position = TimeSpan.Zero;
            BackgroundVideo.Play();
        }

        private void BackgroundVideo_MediaFailed(object sender, ExceptionRoutedEventArgs e)
        {
            MessageBox.Show(
                "Video failed to play:\n" + e.ErrorException.Message,
                "Media error",
                MessageBoxButton.OK,
                MessageBoxImage.Error
            );
        }

        private void MainWindow_Closing(object? sender, CancelEventArgs e)
        {
            // check if were on WaitingPage first because it needs its own cleanup
            Page? currentPage = MainFrame.Content as Page;

            if (currentPage is LoginPage || currentPage is SignUpPage || currentPage is MainPage)
            {
                // not logged in yet, just close
                ClientSession.Communicator.Disconnect();
                return;
            }
            if (currentPage is WaitingPage waitingPage)
            {
                waitingPage.CleanupOnClose();
            }
            else if (currentPage is GameResultsPage resultsPage)
            {
                resultsPage.CleanupOnClose();
            }
            else if(currentPage is GamePage gamePage)
            {
                gamePage.CleanUpOnClose();
            }

            // then always logout
            try
            {
                byte[] requestBuffer = Serializer.SerializeLogoutRequest();
                ClientSession.Communicator.SendEncryptedRequest<LogoutResponse>(requestBuffer);
            }
            catch (Exception ex)
            {
                System.Diagnostics.Debug.WriteLine("Logout failed: " + ex);
            }
            finally
            {
                ClientSession.Communicator.Disconnect();
            }
        }
    }
}