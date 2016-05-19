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
        myTcpClient client;

        public MainWindow()
        {
            InitializeComponent();
            client = new myTcpClient("127.0.0.1", 8282);
        }

        async public void GUI()
        {

        }
        async public void comm()
        {

        }

        private void btnLogin_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
