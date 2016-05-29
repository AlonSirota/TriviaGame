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
        public roomInterface()
        {
            InitializeComponent();
        }
        public roomInterface(myTcpClient newClient)
        {
            InitializeComponent();
            _client = newClient;
        }
        /* NEEDS TO BE DONE AUTOMATICLY
        private void button_Click(object sender, RoutedEventArgs e)
        {
            requestGetUserListAsync();
        }
        private async void requestGetUserListAsync()
        {
            string response = await Task.Factory.StartNew(() => requestUserList());
            string code = response.Substring(0, 3);
            if (response == "106")
            {
                lblStatus.Content = "Correct code detected";
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
        }
        private string requestUserList()
        {
            _client.mySend("205"); //send code
            _userList.Clear();//clear dictionary
            string code = _client.myReceive(3);
            _userNo = Int32.Parse(_client.myReceive(4));
            string nameSize = "";
            string userName = "";
            for (int i = 0; i < _userNo; i++)
            {
                nameSize = _client.myReceive(2);
                userName = _client.myReceive(Int32.Parse(nameSize));
                _userList.Add(userName);
            }
            return code;
        }
        */
    }
}
