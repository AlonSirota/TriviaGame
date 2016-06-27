using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Windows.Threading;
using System.Windows;

namespace client
{
    /// <summary>
    /// Interaction logic for Game.xaml
    /// </summary>
    public partial class Game : Window
    {
        myTcpClient _client;
        int _ansIndex = 0;
        int _time;
        string _question;
        string _ans1;
        string _ans2;
        string _ans3;
        string _ans4;
        int _timePerQuestion;
        DispatcherTimer _dispatcherTimer;

        int _correctAnswerIndex = 0;
        int _currentQuestionIndex = 0;
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
            _timePerQuestion = time;
            setUpTimer();
            getQuestionFirst();
            asignAnswers();
        }

        public void setUpTimer()
        {
            _dispatcherTimer = new DispatcherTimer();
            _dispatcherTimer.Tick += new EventHandler(dispatcherTimer_Tick);
            _dispatcherTimer.Interval = new TimeSpan(0, 0, 1);
        }

        private void btnAns1_Click(object sender, RoutedEventArgs e)
        {
            _ansIndex = 1;
            _dispatcherTimer.Stop();
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
                _currentQuestionIndex++;
                if (response[3] == '1')
                {
                    _correctAnswerIndex++;
                }
                lblScore.Content = _correctAnswerIndex.ToString() + "/" + _currentQuestionIndex.ToString();
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
            string message = "219" + _ansIndex.ToString() + ((int)_time).ToString().PadLeft(2, '0');
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
                _question = _client.myReceive(stringSize);
                stringSize = Int32.Parse(_client.myReceive(3));
                _ans1 = _client.myReceive(stringSize);
                stringSize = Int32.Parse(_client.myReceive(3));
                _ans2 = _client.myReceive(stringSize);
                stringSize = Int32.Parse(_client.myReceive(3));
                _ans3 = _client.myReceive(stringSize);
                stringSize = Int32.Parse(_client.myReceive(3));
                _ans4 = _client.myReceive(stringSize);
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
            _question = _client.myReceive(stringSize);
            stringSize = Int32.Parse(_client.myReceive(3));
            _ans1 = _client.myReceive(stringSize);
            stringSize = Int32.Parse(_client.myReceive(3));
            _ans2 = _client.myReceive(stringSize);
            stringSize = Int32.Parse(_client.myReceive(3));
            _ans3 = _client.myReceive(stringSize);
            stringSize = Int32.Parse(_client.myReceive(3));
            _ans4 = _client.myReceive(stringSize);
        }

        private void asignAnswers()
        {
            btnAns1.Content = _ans1;
            btnAns2.Content = _ans2;
            btnAns3.Content = _ans3;
            btnAns4.Content = _ans4;
            lblQuestion.Content = _question;
            //timer
            _time = _timePerQuestion;
            _dispatcherTimer.Start();
        }
        private void dispatcherTimer_Tick(object sender, EventArgs e)
        {
            _time--;
            if(_time != 0)
            {
                lblTime.Content = _time;
            }
            else
            {
                _ansIndex = 5;
                sendAns();
            }
        }

        private void btnAns2_Click(object sender, RoutedEventArgs e)
        {
            _ansIndex = 2;
            _dispatcherTimer.Stop();
            sendAns();
        }

        private void btnAns3_Click(object sender, RoutedEventArgs e)
        {
            _ansIndex = 3;
            _dispatcherTimer.Stop();
            sendAns();
        }

        private void btnAns4_Click(object sender, RoutedEventArgs e)
        {
            _ansIndex = 4;
            _dispatcherTimer.Stop();
            sendAns();
        }

        private void btnLeaveGame_Click(object sender, RoutedEventArgs e)
        {
            leaveGame();
            Close();
        }
        
        private async void leaveGame()
        {
            //await Task.Factory.StartNew(() => _client.mySend("222"));
            _client.mySend("222");
        }
    }
}
