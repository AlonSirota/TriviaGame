using System;
using System.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Threading;

namespace client
{
    /// <summary>
    /// Interaction logic for createRoom.xaml
    /// </summary>
    public partial class createRoom : Window
    {
        myTcpClient _client;
        public bool _gotParameters;
        public createRoom(myTcpClient client)
        {
            InitializeComponent();
            _client = client;
        }

        private void btnCreateRoom_Click(object sender, RoutedEventArgs e)
        {
            //TODO make sure fields are filled properly (no minuses, second max 99 and such).
            if(this.txtbRoomName.Text == "" || this.txtbPlayerNo.Text == "" || this.txtbQuestionsNo.Text == "" || this.txtbQuestionsNo.Text == "")
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
