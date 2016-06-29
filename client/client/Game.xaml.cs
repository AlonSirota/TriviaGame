using System;
using System.Collections.Generic;
using System.Threading.Tasks;
using System.Windows.Threading;
using System.Windows;
using System.Threading;

namespace client
{
    /// <summary>
    /// Interaction logic for Game.xaml
    /// </summary>
    public partial class Game : Window
    {
        myTcpClient _client;
        int _ansIndex = 0; //TODO try to get rid of this member. could be a local variable.
        int _time;
        string _question;
        string _ans1;
        string _ans2;
        string _ans3;
        string _ans4;
        int _timePerQuestion;
        DispatcherTimer _dispatcherTimer;
        bool _exists;

        int _correctAnswerIndex = 0;
        int _currentQuestionIndex = 0;
        int _totalNumberOfQuestions; //TODO start using finals.
        //for score
        List<string> _userList = new List<string>();
        List<int> _scores = new List<int>();
        int _userNo;

        public Game()
        {
            InitializeComponent();
        }

        public void listenToReplies()
        {
            Application.Current.Dispatcher.BeginInvoke(DispatcherPriority.Background, new Action(async () =>
            {
                string responseCode;

                do /*while (exists)*/
                {
                    responseCode = await Task.Factory.StartNew(() => _client.myReceive(3));
                    //responseCode = _client.myReceive(3);

                    switch (responseCode)
                    {
                        case "120":
                            _currentQuestionIndex++;
                            bool correctAnswer = (await Task.Factory.StartNew(() => _client.myReceive(1))) == "1";
                            if (correctAnswer)
                            {
                                _correctAnswerIndex++;
                            }
                            lblScore.Content = _correctAnswerIndex.ToString() + "/" + _currentQuestionIndex.ToString(); //updates GUI to show status.
                            break;
                        case "118": //TODO use a loop for this mess
                            handleAnswers();
                            break;
                        case "121":
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
                            lblStatus.Content = "Game Ended";
                            //display scores
                            string msg = "";
                            for (int i = 0; i < _userNo; i++)
                            {
                                msg += _userList[i];
                                msg += " : ";
                                msg += _scores[i].ToString();
                                msg += "\n";
                            }
                            MessageBox.Show(msg, "Scores");
                            Close();
                            break;
                        default:
                            lblStatus.Content = "Error - wrong code detected";
                            break;
                    }
                } while (_exists);
            })); //end of beginInvoke.
        }

        public Game(myTcpClient newClient, int timePerQuestion, int numberOfQuestions)
        {
            InitializeComponent();

            _client = newClient;
            _timePerQuestion = timePerQuestion;
            _totalNumberOfQuestions = numberOfQuestions;

            Thread listenThread = new Thread(new ThreadStart(this.listenToReplies));
            listenThread.Start();
            setUpTimer();
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
            sendAnswer();
        }

        private void sendAnswer()
        {
            string message = "219" + _ansIndex.ToString() + ((int)_time).ToString().PadLeft(2, '0');
            _client.mySend(message);
        }

        private void handleAnswers()
        {
            getAnswers();
            asignAnswers();
        }

        private void getAnswers()
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
                sendAnswer();
            }
        }

        private void btnAns2_Click(object sender, RoutedEventArgs e)
        {
            _ansIndex = 2;
            _dispatcherTimer.Stop();
            sendAnswer();
        }

        private void btnAns3_Click(object sender, RoutedEventArgs e)
        {
            _ansIndex = 3;
            _dispatcherTimer.Stop();
            sendAnswer();
        }

        private void btnAns4_Click(object sender, RoutedEventArgs e)
        {
            _ansIndex = 4;
            _dispatcherTimer.Stop();
            sendAnswer();
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
