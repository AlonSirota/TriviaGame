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
using System.Windows.Shapes;

namespace client
{
    /// <summary>
    /// Interaction logic for Game.xaml
    /// </summary>
    public partial class Game : Window
    {
        myTcpClient _client;
        public Game()
        {
            InitializeComponent();
        }
        public Game(myTcpClient newClient)
        {
            InitializeComponent();
            _client = newClient;
        }

        private void btnAns1_Click(object sender, RoutedEventArgs e)
        {

        }
        private async void sendAnswer()
        {
            string response = await Task.Factory.StartNew(() => sendAnswer());
            string code = response.Substring(0, 3);
            while (this.Visibility == Visibility.Visible)
            {
                if (code == "108")
                {
                    lblStatus.Content = "Correct code detected";
                }
                else if (code == "116")
                {
                    lblStatus.Content = "Room Closed By Admin";
                    Close();
                }
                else if (code == "118")
                {
                    //game begun
                }
                else if (code == "112")
                {
                    Close();
                }
                else
                {
                    lblStatus.Content = "Error - wrong code detected";
                }
                if (_userList.Count() == 0)
                {
                    lblStatus.Content = "Error - no rooms";
                }
                else
                {
                    listViewUsers.Items.Clear();
                    foreach (var item in _userList)
                    {
                        listViewUsers.Items.Add(item);
                    }
                }
                response = await Task.Factory.StartNew(() => requestUserList());
                code = response.Substring(0, 3);
            }
        }
    }
}
