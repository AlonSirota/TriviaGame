﻿using System;
using System.Diagnostics;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Threading;
using System.Threading.Tasks;
using System.Threading;

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
        int _numberOfRooms;
        //for creating new room.
        //TODO there has got to be a better solution than this:
        public static string roomName { get; set; }
        public static string playerNo { get; set; }
        public static string questionNo { get; set; }
        public static string questionTime { get; set; }
        public static bool gotInfo { get; set; }


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
            Thread listenThread = new Thread(new ThreadStart(this.listenToReplies));
            listenThread.Start();
        }

        public void listenToReplies()
        {
            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Background, new Action(async () => 
            {
                bool exists = true;
                string responseCode;

                do /*while (exists)*/
                {
                    responseCode = await Task.Factory.StartNew(() => _client.myReceive(3));
                    //responseCode = _client.myReceive(3);

                    switch (responseCode)
                    {
                        case "106":
                            handleRoomList();
                            break;
                        case "114":
                            exists = !handleCreateRoomReply();
                            break;
                        case "110":
                            exists = !handleJoinRoomReply();
                            break;
                        default:
                            lblStatus.Content = "Error - wrong code detected";
                            break;
                    }
                } while (exists);
            })); //end of beginInvoke.
        }

        public void handleRoomList()
        {
            _numberOfRooms = Int32.Parse(_client.myReceive(4));
            string roomId = "";
            string nameSize = "";
            string roomName = "";
            _roomNametoId.Clear();//clear dictionary
            for (int i = 0; i < _numberOfRooms; i++)
            {
                roomId = _client.myReceive(4);
                nameSize = _client.myReceive(2);
                roomName = _client.myReceive(Int32.Parse(nameSize));
                _roomNametoId.Add(roomName, roomId);
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

        private void btnGetRoomList_Click(object sender, RoutedEventArgs e)
        {
            requestGetRoomListAsync();
            lblStatus.Content = "Trying to get Room List";
        }
        private async void requestGetRoomListAsync()
        {
            //string response = await Task.Factory.StartNew(() => requestRoomList());
            requestRoomList();
        }
        //205
        private void requestRoomList()
        {
            _client.mySend("205"); //send code
        }

        private void btnCreateRoom_Click(object sender, RoutedEventArgs e)
        {
            createRoomAsync();
        }

        public bool handleCreateRoomReply()
        {
            string roomId;
            int roomIdLength = Int32.Parse(_client.myReceive(1));
            if (roomIdLength == 0)
            {
                lblStatus.Content = "room creation failed";
                return false;
            }
            else
            {
                roomId = _client.myReceive(roomIdLength);
                //success
                Hide();
                roomInterface roomIn = new roomInterface(_client, Int32.Parse(questionTime), roomId);
                roomIn.ShowDialog();
                lblStatus.Content = "success";
                Show();
                return true;
            }
        }

        private async void createRoomAsync()
        {
            //get information from createRoom window
            Hide();
            createRoom room = new createRoom();
            room.ShowDialog();
            Show();
            if (gotInfo) //if user entered values in the 4 text boxes.
            {
                lblStatus.Content = "got input for createRoom";
                //send create room message and get answer
                //string response = await Task.Factory.StartNew(() => requestCreateRoom());
                requestCreateRoom();
                //string code = response.Substring(0, 3);
                //int roomIdLength = response[3];

                
            }
        }

        private void requestCreateRoom()
        {
            string sendString = "213" + roomName.Length.ToString().PadLeft(2, '0') +roomName +
                    playerNo + questionNo.ToString().PadLeft(2, '0') +
                    questionTime.ToString().PadLeft(2, '0');
            _client.mySend(sendString);
        }

        //TODO not joining properly. potential problem in server
        private void btnJoinRoom_Click(object sender, RoutedEventArgs e)
        {
            joinRoomAsync();
        }

        private bool handleJoinRoomReply()
        {
            int successCode = Int32.Parse(_client.myReceive(1));

            if (successCode == 0)
            {
                //success
                Hide();
                roomInterface room = new roomInterface(_client, Int32.Parse(_joinRoomQuestionTime), _roomNametoId[_roomName]);
                room.ShowDialog();
                lblStatus.Content = "success";
                Show();
                return true;
            }
            else if (successCode == 1)
            {
                lblStatus.Content = "Error - room is full";
                return false;
            }
            else if (successCode == 2)
            {
                lblStatus.Content = "Error - room doesnt exist";
                return false;
            }
            else
            {
                lblStatus.Content = "Error - unexpected protocol code recieved";
                return false;
            }
        }

        private /*async*/ void joinRoomAsync()
        {
            if (_roomName == null)
            {
                lblStatus.Content = "Choose a room";
            }
            else
            {
                string roomID = _roomNametoId[_roomName]; //get room id
                //string response = await Task.Factory.StartNew(() => requestJoinRoom(roomID));
                requestJoinRoom(roomID);
            }            
        }

        //TODO change protocol to 208<idLength><id>
        private void requestJoinRoom(string roomID)
        {
            _client.mySend("209"+roomID);
        }

        private void listViewRooms_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            _roomName = (string)listViewRooms.SelectedItem;
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            //sign out
            signout();
        }
        private async void signout()
        {
            //await Task.Factory.StartNew(() => _client.mySend("201"));
            _client.mySend("201");
        }
        private void btnBestScores_Click(object sender, RoutedEventArgs e)
        {
            Hide();
            bestScores statusWin = new bestScores(_client);
            statusWin.ShowDialog();
            lblStatus.Content = "success";
            Show();
        }

        private void btnMyStatus_Click(object sender, RoutedEventArgs e)
        {
            Hide();
            status statusWin = new status(_client);
            statusWin.ShowDialog();
            lblStatus.Content = "success";
            Show();
        }
    }
}
