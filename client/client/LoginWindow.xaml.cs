using System.Windows;
using System.Windows.Controls;

namespace client
{
    /// <summary>
    /// Interaction logic for LoginWindow.xaml
    /// </summary>
    public partial class LoginWindow : Window
    {
        myTcpClient _client;
        public LoginWindow()
        {
            InitializeComponent();
            _client = new myTcpClient("127.0.0.1", 8820);
        }

        /*
         * tries to sign in.
         * 200<user length><username><pass length><password>
        */
        private void btnLogin_Click(object sender, RoutedEventArgs e)
        {
            requestSignInAsync();
            lblStatus.Content = "trying to login";
        }

        private /*async*/ void requestSignInAsync()
        {
            //string response = await Task.Factory.StartNew(() => requestSignIn());
            string response = requestSignIn();

            if (response == "1020")
            {
                //open new window.
                Hide();
                MainMenu s = new MainMenu(_client);
                s.ShowDialog();
                Close();
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
            _client.mySend("200" +
                this.txtUsername.Text.Length.ToString().PadLeft(2, '0') + this.txtUsername.Text +
                this.passBox.Password.Length.ToString().PadLeft(2, '0') + this.passBox.Password);
            return _client.myReceive(4);
        }

        private void btnSignUp_Click(object sender, RoutedEventArgs e)
        {
            Hide();
            SignUp s = new SignUp(this.txtUsername.Text, this.passBox.Password, _client);
            s.ShowDialog();
            Show();
        }

        private void btnExit_Click(object sender, RoutedEventArgs e)
        {
            _client.mySend("299");
            Close();
        }
    }
}
