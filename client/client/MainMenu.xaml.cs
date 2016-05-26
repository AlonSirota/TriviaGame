﻿using System;
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
        public string roomName { get; set; }
        public string playerNo { get; set; }
        public string questionNo { get; set; }
        public string questionTime { get; set; }
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
            if (response == "106")
            {
                lblStatus.Content = "Correct code detected";
            }
            else
            {
                lblStatus.Content = "Error - wrong code detected";
            }
            if(_roomNametoId.Count() == 0)
            {
                lblStatus.Content = "Error - no rooms";
            }
            else
            {
                listViewRooms.Items.Clear();
                foreach (var item in _roomNametoId.Values)
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
            for(int i = 0; i < _roomNo;i++)
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

            //send create room message
            string sendString = "213" + roomName.Length.ToString().PadLeft(2, '0');
            sendString += playerNo + questionNo.ToString().PadLeft(2, '0');
            sendString += questionTime.ToString().PadLeft(2, '0');
            _client.mySend(sendString);
            //get result from server

            //open Room window - lets the user start the game or close it


            //return "y";
        }
    }
}
