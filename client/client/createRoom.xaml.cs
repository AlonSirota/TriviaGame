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
        public bool _gotParameters;
        public createRoom()
        {
            InitializeComponent();
        }

        private void btnCreateRoom_Click(object sender, RoutedEventArgs e)
        {
            //TODO make sure fields are filled properly (no minuses, second max 99 and such).
            if(this.txtbRoomName.Text == "" || this.txtbPlayerNo.Text == "" || this.txtbQuestionsNo.Text == "" || this.txtbQuestionTime.Text == "")
            {
                MessageBox.Show("Complete all fields");
            }
            else
            {
                _gotParameters = true;
                Close();
            }
        }

        private void btnBack_Click(object sender, RoutedEventArgs e)
        {
            _gotParameters = false;
            Close();
        }
    }
}
