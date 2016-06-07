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
    /// Interaction logic for MainMenu.xaml
    /// </summary>
    public partial class MainMenu : Window
    {
        myTcpClient _client;
        //for getting existing rooms
        Dictionary<string, string> _roomNametoId = new Dictionary<string, string>();
        int _roomNo;
        //for creating new room
        public static string roomName { get; set; }
        public static string playerNo { get; set; }
        public static string questionNo { get; set; }
        public static string questionTime { get; set; }
        

        //joinRoom variables
        string _joinRoomQuestionsNo;
        string _joinRoomQuestionTime;
        string _roomName;

        public MainMenu()
        {
            InitializeComponent();
        }
        public MainMenu(myTcpClient newClient)
        {
            InitializeComponent();
            _client = newClient;
        }

        private void btnGetRoomList_Click(object sender, RoutedEventArgs e)
        {
            requestGetRoomListAsync();
            lblStatus.Content = "Trying to get Room List";
        }
        private async void requestGetRoomListAsync()
        {
            string response = await Task.Factory.StartNew(() => requestRoomList());
            string code = response.Substring(0, 3);
            if (code == "106")
            {
                lblStatus.Content = "Correct code detected";
            }
            else
            {
                lblStatus.Content = "Error - wrong code detected";
            }
            if (_roomNametoId.Count() == 0)
            {
                lblStatus.Content = "Error - no rooms";
            }
            else
            {
                listViewRooms.Items.Clear();
                foreach (var item in _roomNametoId.Keys)
                {
                    listViewRooms.Items.Add(item);
                }
            }
        }
        //205
        private string requestRoomList()
        {
            _client.mySend("205"); //send code
            _roomNametoId.Clear();//clear dictionary
            string code = _client.myReceive(3);
            _roomNo = Int32.Parse(_client.myReceive(4));
            string roomId = "";
            string nameSize = "";
            string roomName = "";
            for (int i = 0; i < _roomNo; i++)
            {
                roomId = _client.myReceive(4);
                nameSize = _client.myReceive(2);
                roomName = _client.myReceive(Int32.Parse(nameSize));
                _roomNametoId.Add(roomName, roomId);
            }
            return code;
        }

        private void btnCreateRoom_Click(object sender, RoutedEventArgs e)
        {
            createRoomAsync();
        }

        private async void createRoomAsync()
        {
            //get information from createRoom window
            Hide();
            createRoom room = new createRoom();
            room.ShowDialog();
            lblStatus.Content = "got input for createRoom";
            Show();
            //send create room message and get answer
            string response = await Task.Factory.StartNew(() => requestCreateRoom());
            string code = response.Substring(0, 3);
            if (code == "114")
            {
                lblStatus.Content = "Correct code detected";//open roomInterfaceMaster
                if (response[3] == '0')
                {
                    //success
                    Hide();
                    roomInterfaceMaster roomIn = new roomInterfaceMaster(_client);
                    roomIn.ShowDialog();
                    lblStatus.Content = "success";
                    Show();
                }
                else if (response[3] == '1')
                {
                    lblStatus.Content = "Error - fail";
                }
                else
                {
                    lblStatus.Content = "Error - unknown";
                }
            }
            else
            {
                lblStatus.Content = "Error - wrong code detected";
            }
        }

        private string requestCreateRoom()
        {
            string sendString = "213" + roomName.Length.ToString().PadLeft(2, '0') +roomName +
                    playerNo + questionNo.ToString().PadLeft(2, '0') +
                    questionTime.ToString().PadLeft(2, '0');
            _client.mySend(sendString);
            string ans = _client.myReceive(4);
            return ans;
        }

        private void btnJoinRoom_Click(object sender, RoutedEventArgs e)
        {
            joinRoomAsync();
        }
        private async void joinRoomAsync()
        {
            string roomID = _roomNametoId[_roomName]; //get room id
            string response = await Task.Factory.StartNew(() => requestJoinRoom(roomID));
            string code = response.Substring(0, 3);
            if (code == "110")
            {
                lblStatus.Content = "Correct code detected";//open roomInterface
                if(response[3] == '0')
                {
                    //success
                    Hide();
                    roomInterface room = new roomInterface(_client);
                    room.ShowDialog();
                    lblStatus.Content = "success";
                    Show();
                }
                else if (response[3] == '1')
                {
                    lblStatus.Content = "Error - room is full";
                }
                else if (response[3] == '2')
                {
                    lblStatus.Content = "Error - room doesnt exist";
                }
                else
                {
                    lblStatus.Content = "Error - unknown";
                }
            }
            else
            {
                lblStatus.Content = "Error - wrong code detected";
            }
        }
        private string requestJoinRoom(string roomID)
        {
            _client.mySend("209"+roomID); //send code
            string code = _client.myReceive(3);
            string ans = _client.myReceive(1);
            if(ans == "0" && code == "110")
            {
                _joinRoomQuestionsNo = _client.myReceive(2);
                _joinRoomQuestionTime = _client.myReceive(2);
            }
            return code+ans;
        }

        private void listViewRooms_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            _roomName = (string)listViewRooms.SelectedItem;
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            //sign out
        }
    }
}
