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
    /// Interaction logic for SignUp.xaml
    /// </summary>
    public partial class SignUp : Window
    {
        myTcpClient _client;
        public SignUp(string username, string password, myTcpClient newClient)
        {
            InitializeComponent();
            this.txtUsername.Text = username;
            this.txtPassword.Text = password;
            _client = newClient;
        }

        private void btnBack_Click(object sender, RoutedEventArgs e)
        {
            this.Close();
        }

        private void btnSignUp_Click(object sender, RoutedEventArgs e)
        {
            requestSignUpAsync();
            lblStatus.Content = "trying to sign up";
        }

        private async void requestSignUpAsync()
        {
            string response = await Task.Factory.StartNew(() => requestSignUp());

            if (response == "1040")
            {
                lblStatus.Content = "sign up succeeded";
            }
            else if (response == "1041")
            {
                lblStatus.Content = "sign up failed: invalid password";
            }
            else if (response == "1042")
            {
                lblStatus.Content = "sign up failed: username taken";
            }
            else if (response == "1043")
            {
                lblStatus.Content = "sign up failed: invalid username";
            }
            else if (response == "1044")
            {
                lblStatus.Content = "sign up failed: other";
            }
            else
            {
                lblStatus.Content = "error";
            }
        }

        //203##username##password##email
        private string requestSignUp()
        {
            _client.mySend(
                "203" +
                txtUsername.Text.Length.ToString().PadLeft(2, '0') +
                txtUsername.Text +
                txtPassword.Text.Length.ToString().PadLeft(2, '0') +
                txtPassword.Text +
                txtEmail.Text.Length.ToString().PadLeft(2, '0') +
                txtEmail.Text);

            return _client.myReceive(4);
        }
    }
}
