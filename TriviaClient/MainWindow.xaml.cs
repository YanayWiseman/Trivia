using System;
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
    }
}