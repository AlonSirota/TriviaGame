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
                MainMenu._createRoomNumberOfQuestions = this.txtbQuestionsNo.Text;
                MainMenu._createRoomTimePerQuestion = this.txtbQuestionTime.Text;
                MainMenu._createRoomName = this.txtbRoomName.Text;
                MainMenu._createRoomNumberOfPlayers = this.txtbPlayerNo.Text;
                MainMenu._createRoomGotInfo = true;
                Close();
            }
        }

        private void btnBack_Click(object sender, RoutedEventArgs e)
        {
            MainMenu._createRoomGotInfo = false;
            Close();
        }
    }
}
