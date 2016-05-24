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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace client
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        myTcpClient _client;

        public MainWindow()
        {
            InitializeComponent();
            _client = new myTcpClient("127.0.0.1", 8282);
        }

        async public void GUI()
        {

        }
        async public void comm()
        {

        }

        //We might want to use a password box
        /*
         * tries to sign in.
         * 200<user length><username><pass length><password>
        */
        private void btnLogin_Click(object sender, RoutedEventArgs e)
        {
            requestSignInAsync();
            lblStatus.Content = "trying to login";
        }

        private async void requestSignInAsync()
        {
            string response = await Task.Factory.StartNew(() => requestSignIn());

            if (response == "1020")
            {
                //open new window.
                lblStatus.Content = "Login succeeded";
            }
            else if (response == "1021")
            {
                lblStatus.Content = "Login failed: wrong username or password";
            }
            else if (response == "1022")
            {
                lblStatus.Content = "Login failed: user already logged in";
            }
            else
            {
                lblStatus.Content = "error";
            }
        }

        private string requestSignIn()
        {
            _client.mySend(
                "200" +
                txtUsername.Text.Length.ToString().PadLeft(2, '0') +
                txtUsername.Text +
                txtPassword.Text.Length.ToString().PadLeft(2, '0') +
                txtPassword.Text);

            return _client.myReceive(4);
        }

        private void btnSignUp_Click(object sender, RoutedEventArgs e)
        {
            SignUp s = new SignUp(txtUsername.Text, txtPassword.Text, _client);
            s.ShowDialog();
        }
    }
}
