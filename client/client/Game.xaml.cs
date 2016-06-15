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

        int correctNo = 0;
        int questionNo = 0;
        //for score
        List<string> _userList = new List<string>();
        List<int> _scores = new List<int>();
        int _userNo;

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
            getQuestionFirst();
            asignAnswers();
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
                questionNo++;
                if (response[3] == '1')
                {
                    correctNo++;
                }
                lblScore.Content = correctNo.ToString() + "/" + questionNo.ToString();
                //get question
                response = await Task.Factory.StartNew(() => getQuestion());
                code = response.Substring(0, 3);
                if (code == "118")
                {
                    //got question
                    asignAnswers();
                }
                else if (code == "121")
                {
                    lblStatus.Content = "Game Ended";
                    //display scores
                    string msg = "";
                    for(int i = 0; i < _userNo; i++)
                    {
                        msg += _userList[i];
                        msg += " : ";
                        msg += _scores[i].ToString();
                        msg += "\n";
                    }
                    MessageBox.Show(msg,"Scores");
                    Close();
                }
                else
                {
                    lblStatus.Content = "Error - wrong code detected";
                }
            }
            else
            {
                lblStatus.Content = "Error - expected '120' but recieved: " + code;
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
            else if(code == "121")
            {
                _userNo = Int32.Parse(_client.myReceive(1));
                string nameSize = "";
                string userName = "";
                for (int i = 0; i < _userNo; i++)
                {
                    nameSize = _client.myReceive(2);
                    userName = _client.myReceive(Int32.Parse(nameSize));
                    _userList.Add(userName);
                    _scores.Add(Int32.Parse(_client.myReceive(2)));
                }
            }
            return (code);
        }
        private void getQuestionFirst()
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

        private void btnLeaveGame_Click(object sender, RoutedEventArgs e)
        {
            leaveGame();
            Close();
        }
        
        private async void leaveGame()
        {
            await Task.Factory.StartNew(() => _client.mySend("222"));
        }
    }
}
