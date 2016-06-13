using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Diagnostics;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Net;
using System.Net.Sockets;
using System.Net.Security;
using System.Threading;
using System.Windows.Threading;
namespace client
{
    public class myTcpClient
    {
        TcpClient client;
        IPEndPoint serverEndPoint;
        NetworkStream clientStream;
        //SslStream secureStream;
        public myTcpClient(string ipAddress, int port)
        {
            client = new TcpClient();
            serverEndPoint = new IPEndPoint(IPAddress.Parse(ipAddress), port);

            client.Connect(serverEndPoint);
            clientStream = client.GetStream();
            //secure
            //secureStream = new SslStream(client.GetStream());
            //client.Close();
        }

        public void mySend(string text)
        {
            byte[] buffer = new ASCIIEncoding().GetBytes(text);
            clientStream.Write(buffer, 0, buffer.Length);
            clientStream.Flush();
            //secure
            //secureStream.Write(buffer);
            //secureStream.Flush();
        }

        public string myReceive(int size)
        {
            byte[] buffer = new byte[size];
            clientStream.Read(buffer, 0, size);
            //secure
            //secureStream.Read(buffer, 0, size);
            return new ASCIIEncoding().GetString(buffer);
            
        }
    }
}