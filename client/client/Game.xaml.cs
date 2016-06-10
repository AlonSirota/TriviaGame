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
        double time;
        bool isAnswered = false;
        string question;
        string ans1;
        string ans2;
        string ans3;
        string ans4;
        int timePerQuestion;

        public Game()
        {
            InitializeComponent();
        }
        public Game(myTcpClient newClient)
        {
            InitializeComponent();
            _client = newClient;
        }

        private void btnAns1_Click(object sender, RoutedEventArgs e)
        {

        }
        private async void sendAns()
        {
            string response = await Task.Factory.StartNew(() => sendAnswer());
            string code = response.Substring(0, 3);
            if (code == "120")
            {
                lblStatus.Content = "Correct code detected";
                //indicate if correct
            }
            else if (code == "121")
            {
                lblStatus.Content = "Game Ended";
                //display scores
                Close();
            }
            else if (code == "118")
            {
                //got question
                asignAnswers();
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

        private void asignAnswers()
        {
            btnAns1.Content = ans1;
            btnAns2.Content = ans2;
            btnAns3.Content = ans3;
            btnAns4.Content = ans4;
            lblQuestion.Content = question;
            //timer
            time = timePerQuestion;
            DispatcherTimer dispatcherTimer = new DispatcherTimer();
            dispatcherTimer.Tick += new EventHandler(dispatcherTimer_Tick);
            dispatcherTimer.Interval = new TimeSpan(0, 0, 1);
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
                isAnswered = true;
                ansNo = 5;
            }
        }
    }
}
