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
    /// Interaction logic for createRoom.xaml
    /// </summary>
    public partial class createRoom : Window
    {
        public string roomName;
        public string playerNo;
        public string questionNo;
        public string questionTime;
        public createRoom()
        {
            InitializeComponent();
        }

        private void txtbQuestionTime_TextChanged(object sender, TextChangedEventArgs e)
        {
            questionTime = ((TextBox)sender).Text;
        }

        private void txtbQuestionsNo_TextChanged(object sender, TextChangedEventArgs e)
        {
            questionNo = ((TextBox)sender).Text;
        }

        private void txtbPlayerNo_TextChanged(object sender, TextChangedEventArgs e)
        {
            playerNo = ((TextBox)sender).Text;
        }

        private void txtbRoomName_TextChanged(object sender, TextChangedEventArgs e)
        {
            roomName = ((TextBox)sender).Text;
        }

        private void btnCreateRoom_Click(object sender, RoutedEventArgs e)
        {
            //move values to Main menu
    
        }
    }
}
