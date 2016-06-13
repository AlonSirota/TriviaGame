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
    /// Interaction logic for roomInterface.xaml
    /// </summary>
    public partial class roomInterface : Window
    {
        myTcpClient _client;
        List<string> _userList = new List<string>();
        int _userNo;
        int _time;
        public roomInterface()
        {
            InitializeComponent();
        }
        public roomInterface(myTcpClient newClient,int time)
        {
            InitializeComponent();
            _client = newClient;
            requestGetUserListAsync();
            _time = time;
        }
        private async void requestGetUserListAsync()
        {
            string response = await Task.Factory.StartNew(() => requestUserList());
            string code = response.Substring(0, 3);
            while (this.Visibility == Visibility.Visible)
            {
                if (code == "108")
                {
                    lblStatus.Content = "Correct code detected";
                }
                else if(code == "116")
                {
                    lblStatus.Content = "Room Closed By Admin";
                    Close();
                }
                else if(code == "118")
                {
                    //game begun
                    Hide();
                    Game s = new Game(_client,_time);
                    s.ShowDialog();
                    Close();//doesnt close   
                }
                else if(code == "112")
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
        private string requestUserList()
        {
            string code = _client.myReceive(3);
            if(code == "108")
            {
                _userNo = Int32.Parse(_client.myReceive(1));
                string nameSize = "";
                string userName = "";
                for (int i = 0; i < _userNo; i++)
                {
                    nameSize = _client.myReceive(2);
                    userName = _client.myReceive(Int32.Parse(nameSize));
                    _userList.Add(userName);
                }
            }
            return code;
        }

        private void btnLeaveRoom_Click(object sender, RoutedEventArgs e)
        {
            requestLeaveRoom();
        }
        private void requestLeaveRoom()
        {
            _client.mySend("211"); //send code
        }
    }
}
