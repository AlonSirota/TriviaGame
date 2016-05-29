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
        string _username = "";
        string _password = "";
        string _email = "";
        public SignUp(string username, string password, myTcpClient newClient)
        {
            InitializeComponent();
            txtUsername.Text = username;
            passBox.Password = password;
            _username = username;
            _password = password;
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
                Close();
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

        //We might want to use a Password Box
        //203##username##password##email
        private string requestSignUp()
        {
            _client.mySend("203" +
                _username.Length.ToString().PadLeft(2, '0') + _username +
                _password.Length.ToString().PadLeft(2, '0') + _password +
                _email.Length.ToString().PadLeft(2, '0') + _email);
            return _client.myReceive(4);
        }

        private void txtUsername_TextChanged(object sender, TextChangedEventArgs e)
        {
            _username = txtUsername.Text;
        }

        private void txtEmail_TextChanged(object sender, TextChangedEventArgs e)
        {
            _email = txtEmail.Text;
        }

        private void passBox_PasswordChanged(object sender, RoutedEventArgs e)
        {
            _password = passBox.Password;
        }
    }
}
