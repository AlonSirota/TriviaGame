﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows;
using System.Threading;
using System.Windows.Threading;
using System.Threading.Tasks;

namespace client
{
    /// <summary>
    /// Interaction logic for roomInterface.xaml
    /// </summary>
    public partial class roomInterface : Window
    {
        myTcpClient _client;
        List<string> _userList = new List<string>();
        int _numberOfQuestions;
        int _timePerQuestion;
        
        public roomInterface()
        {
            InitializeComponent();
        }

        public roomInterface(myTcpClient newClient, int timePerQuestion, int numberOfQuestions)
        {
            _timePerQuestion = timePerQuestion;
            InitializeComponent();
            _client = newClient;
            //TODO? send a join request
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
                    responseCode = await Task.Factory.StartNew(() => _client.myReceive(3)); //TODO wait, using async in a thread is pointless, right? (so we can either stop calling this function on a different thread, or use a non async call to the recieve)
                    //responseCode = _client.myReceive(3);

                    switch (responseCode)
                    {
                        case "108":
                            lblStatus.Content = "recieved user list.";
                            readUserList();
                            break;
                        case "116":
                            lblStatus.Content = "Room Closed By Admin";
                            Close();
                            exists = false;
                            break;
                        case "118":
                            //game begun
                            Hide();
                            Game s = new Game(_client, _timePerQuestion, _numberOfQuestions);
                            s.ShowDialog();
                            Close();
                            exists = false;
                            break;
                        case "112":
                            exists = false;
                            Close();
                            break;
                        default:
                            lblStatus.Content = "Error - wrong code detected";
                            break;
                    }
                } while (exists);
            })); //end of beginInvoke.
        }

        private void btnLeaveRoom_Click(object sender, RoutedEventArgs e)
        {
            requestLeaveRoom();
        }

        private void requestLeaveRoom()
        {
            _client.mySend("211"); //send code
        }

        private void btnStartGame_Click(object sender, RoutedEventArgs e)
        {
            startGame();
        }

        private async void startGame()
        {
            //await Task.Factory.StartNew(() => _client.mySend("217"));
            _client.mySend("217"); //make this async?

            //TODO uncomment this and make it works. its supposed to check with the server if the game actually successfuly started (it could fail if there arent enough questions).
            /*
            string response = _client.myReceive(4); //make these async?
            if (response == "9991")
            {
                lblStatus.Content = "server encountered an error, probably not enough questions.";
            }
            else if (response == "9990")
            {
                lblStatus.Content = "server started game succesfuly - game should start now";
            }*/
        }

        //after recieved 108
        private void readUserList()
        {
            int numberOfUsers = Int32.Parse(_client.myReceive(1));
            string nameSize = "";
            string userName = "";
            _userList.Clear();

            for (int i = 0; i < numberOfUsers; i++)
            {
                nameSize = _client.myReceive(2);
                userName = _client.myReceive(Int32.Parse(nameSize));
                _userList.Add(userName);
            }

            if (_userList.Count() == 0)
            {
                lblStatus.Content = "Error - room doesn't exist";
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
    }
}
