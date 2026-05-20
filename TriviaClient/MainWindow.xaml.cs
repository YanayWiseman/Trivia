using System.ComponentModel;
using System.Text;
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
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();
            this.Closing += MainWindow_Closing;
        }

        private void MainWindow_Closing(object? sender, CancelEventArgs e)
        {
            // check if were on WaitingPage first because it needs its own cleanup
            Page? currentPage = MainFrame.Content as Page;

            if (currentPage is LoginPage || currentPage is SignUpPage || currentPage is MainPage)
            {
                // not logged in yet, just close
                return;
            }
            if (currentPage is WaitingPage waitingPage)
            {
                waitingPage.CleanupOnClose();
            }

            // then always logout
            byte[] requestBuffer = Serializer.SerializeLogoutRequest();
            ClientSession.Communicator.SendRequest<LogoutResponse>(requestBuffer);

            ClientSession.Communicator.Disconnect();
        }
    }
}