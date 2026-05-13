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
    public partial class JoinRoomPage : Page
    {
        public JoinRoomPage()
        {
            InitializeComponent();
            LoadRooms();
        }

        private void LoadRooms()
        {
            RoomsListBox.Items.Clear();

            RoomsListBox.Items.Add("Room 1");
            RoomsListBox.Items.Add("Room 2");
            RoomsListBox.Items.Add("Room 3");
        }

        private void GoBack_Click(object sender, RoutedEventArgs e)
        {
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

            string roomName = RoomsListBox.SelectedItem.ToString();
        }
    }
}