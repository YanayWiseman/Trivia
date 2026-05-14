using System.Configuration;
using System.Data;
using System.Windows;

namespace TriviaClient
{
    public partial class App : Application
    {
        // we are overriding the OnStartup method to initialize the communicator and make it global
        protected override void OnStartup(StartupEventArgs e)
        {
            base.OnStartup(e);

            AppState.Communicator = new Communicator();
        }
    }
}
