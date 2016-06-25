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
        public createRoom()
        {
            InitializeComponent();
        }

        private void btnCreateRoom_Click(object sender, RoutedEventArgs e)
        {
            //move values to Main menu
            if(this.txtbRoomName.Text == "" || this.txtbPlayerNo.Text == "" || this.txtbQuestionsNo.Text == "" || this.txtbQuestionTime.Text == "")
            {
                MessageBox.Show("Complete all fields");
            }
            else
            {
                MainMenu.questionNo = this.txtbQuestionsNo.Text;
                MainMenu.questionTime = this.txtbQuestionTime.Text;
                MainMenu.roomName = this.txtbRoomName.Text;
                MainMenu.playerNo = this.txtbPlayerNo.Text;
                MainMenu.gotInfo = true;
                Close();
            }
        }

        private void btnBack_Click(object sender, RoutedEventArgs e)
        {
            MainMenu.gotInfo = false;
            Close();
        }
    }
}
