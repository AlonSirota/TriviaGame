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
    /// Interaction logic for status.xaml
    /// </summary>
    public partial class status : Window
    {
        myTcpClient _client;
        int gameNo = 0;
        int correctAnsNo = 0;
        int wrongAnsNo = 0;
        double avgTime = 0;
        public status()
        {
            InitializeComponent();
        }
        
        public status(myTcpClient client)
        {
            InitializeComponent();
            _client = client;
            getStatus();
        }
        
        private async void getStatus()
        {
            //await Task.Factory.StartNew(() => requestGetStatus());
            requestGetStatus();
            lblGameNo.Content = "Total Game No : " + gameNo.ToString();
            lblCorrectNo.Content = "Number of correct answers : " + correctAnsNo.ToString();
            lblWrongNo.Content = "Number of wrong answers : " + wrongAnsNo.ToString();
            lblAvgTime.Content = "Average time per question : " + avgTime.ToString();
        }

        private void requestGetStatus()
        {
            _client.mySend("225"); //send code
            string code = _client.myReceive(3);
            if(code == "126")
            {
                gameNo = Int32.Parse(_client.myReceive(4));
                if(gameNo == 0)
                {
                    //no games
                    return;
                }
                correctAnsNo = Int32.Parse(_client.myReceive(6));
                wrongAnsNo = Int32.Parse(_client.myReceive(6));
                avgTime = Int32.Parse(_client.myReceive(2));
                avgTime += Double.Parse(_client.myReceive(2))/100;
            }
        }

        private void btnExit_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}
