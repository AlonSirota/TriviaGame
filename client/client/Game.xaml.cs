using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Threading;
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
        int ansNo = 0;
        int time;
        string question;
        string ans1;
        string ans2;
        string ans3;
        string ans4;
        int timePerQuestion;
        DispatcherTimer dispatcherTimer;

        public Game()
        {
            InitializeComponent();
        }
        public Game(myTcpClient newClient,int time)
        {
            InitializeComponent();
            _client = newClient;
            timePerQuestion = time;
            dispatcherTimer = new DispatcherTimer();
            dispatcherTimer.Tick += new EventHandler(dispatcherTimer_Tick);
            dispatcherTimer.Interval = new TimeSpan(0, 0, 1);
        }

        private void btnAns1_Click(object sender, RoutedEventArgs e)
        {
            ansNo = 1;
            dispatcherTimer.Stop();
            sendAns();
        }
        private async void sendAns()
        {
            string response = await Task.Factory.StartNew(() => sendAnswer());
            string code = response.Substring(0, 3);
            if (code == "120")
            {
                lblStatus.Content = "Correct code detected";
                //indicate if correct

                //get question
                response = await Task.Factory.StartNew(() => getQuestion());
                code = response.Substring(0, 3);
                if (code == "118")
                {
                    //got question
                    asignAnswers();
                }
            }
            else if (code == "121")
            {
                lblStatus.Content = "Game Ended";
                //display scores
                Close();
            }
            else
            {
                lblStatus.Content = "Error - wrong code detected";
            }
        }

        private string sendAnswer()
        {
            string message = "219" + ansNo.ToString() + ((int)time).ToString().PadLeft(2, '0');
            _client.mySend(message);
            string code = _client.myReceive(3);
            string ans = "";
            if(code == "120")
            {
                ans = _client.myReceive(1);
            }
            return code + ans;
        }

        private string getQuestion()
        {
            string code = _client.myReceive(3);
            if(code == "118")
            {
                int stringSize = Int32.Parse(_client.myReceive(3));
                question = _client.myReceive(stringSize);
                stringSize = Int32.Parse(_client.myReceive(3));
                ans1 = _client.myReceive(stringSize);
                stringSize = Int32.Parse(_client.myReceive(3));
                ans2 = _client.myReceive(stringSize);
                stringSize = Int32.Parse(_client.myReceive(3));
                ans3 = _client.myReceive(stringSize);
                stringSize = Int32.Parse(_client.myReceive(3));
                ans4 = _client.myReceive(stringSize);
            }
            return (code);
        }

        private void asignAnswers()
        {
            btnAns1.Content = ans1;
            btnAns2.Content = ans2;
            btnAns3.Content = ans3;
            btnAns4.Content = ans4;
            lblQuestion.Content = question;
            //timer
            time = timePerQuestion;
            dispatcherTimer.Start();
        }
        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            time--;
            if(time != 0)
            {
                lblTime.Content = time;
            }
            else
            {
                ansNo = 5;
                sendAns();
            }
        }

        private void btnAns2_Click(object sender, RoutedEventArgs e)
        {
            ansNo = 2;
            dispatcherTimer.Stop();
            sendAns();
        }

        private void btnAns3_Click(object sender, RoutedEventArgs e)
        {
            ansNo = 3;
            dispatcherTimer.Stop();
            sendAns();
        }

        private void btnAns4_Click(object sender, RoutedEventArgs e)
        {
            ansNo = 4;
            dispatcherTimer.Stop();
            sendAns();
        }
    }
}
