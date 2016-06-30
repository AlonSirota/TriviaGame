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
    /// Interaction logic for bestScores.xaml
    /// </summary>
    public partial class bestScores : Window
    {
        myTcpClient _client;
        List<string> _users = new List<string>();
        List<int> _scores = new List<int>();
        public bestScores()
        {
            InitializeComponent();
        }
        public bestScores(myTcpClient client)
        {
            InitializeComponent();
            _client = client;
            getBestScores();
        }

        private async void getBestScores()
        {
            //await Task.Factory.StartNew(() => requestGetBestScores());
            requestGetBestScores();
            if ((string)lblScore1.Content != "")
            {
                lblScore1.Content = _users[0] + " : " + _scores[0];
            }
            if ((string)lblScore2.Content != "")
            {
                lblScore2.Content = _users[1] + " : " + _scores[1];
            }
            if ((string)lblScore3.Content != "")
            {
                lblScore3.Content = _users[2] + " : " + _scores[2];
            }
        }

        private void requestGetBestScores()
        {
            int nameSize = 0;
            string username = "";
            for (int i = 0; i < 3; i++)
            {
                nameSize = Int32.Parse(_client.myReceive(2));
                username = _client.myReceive(nameSize);
                _users.Add(username);
                _scores.Add(Int32.Parse(_client.myReceive(6)));
            }
        }

        private void btnClose_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}
