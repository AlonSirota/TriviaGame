using System;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Threading;

namespace client
{
    /// <summary>
    /// Interaction logic for createRoom.xaml
    /// </summary>
    public partial class createRoom : Window
    {
        myTcpClient _client;
        Thread _listenThread;
        public createRoom(myTcpClient client)
        {
            InitializeComponent();
            _client = client;
            _listenThread = new Thread(new ThreadStart(this.listenToReplies));
            _listenThread.Start();
        }

        private void btnCreateRoom_Click(object sender, RoutedEventArgs e)
        {
            //TODO make sure fields are filled properly (no minuses, second max 99 and such).
            if(this.txtbRoomName.Text == "" || this.txtbPlayerNo.Text == "" || this.txtbQuestionsNo.Text == "" || this.txtbQuestionsNo.Text == "")
            {
                MessageBox.Show("Complete all fields");
            }
            else
            {
                requestCreateRoom();
            }
        }

        private void btnBack_Click(object sender, RoutedEventArgs e)
        {
            _listenThread.Join();
            Close();
        }

        //213##roomName playersNumber questionsNumber questionTimeInSec
        private void requestCreateRoom()
        {
            string sendString = "213" +
                this.txtbRoomName.Text.Length.ToString().PadLeft(2, '0') +
                this.txtbRoomName.Text +
                this.txtbPlayerNo.Text +
                this.txtbQuestionsNo.Text.PadLeft(2, '0') +
                this.txtbQuestionTime.Text.PadLeft(2, '0');

            _client.mySend(sendString);
        }

        public bool handleCreateRoomReply()
        {
            string success = _client.myReceive(1);
            if (success == "1")
            {
                lblStatus.Content = "room creation failed";
                return false;
            }
            else if (success == "0")
            {
                //success
                Hide();
                roomInterface roomIn = new roomInterface(_client, Int32.Parse(this.txtbQuestionTime.Text), Int32.Parse(this.txtbQuestionsNo.Text));
                roomIn.ShowDialog();
                lblStatus.Content = "success";
                Show();
                return true;
            }
            else
            {
                lblStatus.Content = "Error: unexepected code recieved";
                return false;
            }
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
                        case "114":
                            exists = !handleCreateRoomReply();
                            break;
                        default:
                            lblStatus.Content = "Error - wrong code detected";
                            break;
                    }
                } while (exists);
            })); //end of beginInvoke.
        }
    }
}
