using System.Text;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System;
using System.Windows;

namespace client
{
    public class myTcpClient
    {
        TcpClient _client;
        IPEndPoint _serverEndPoint;
        NetworkStream _clientStream;
        //SslStream secureStream;

        public myTcpClient()
        {
            _client = new TcpClient();
            _serverEndPoint = null;
            _clientStream = null;
            //secure
            //secureStream = new SslStream(client.GetStream());
            //client.Close();
        }

        public bool connect(string ipAddress, int port)
        {
            _serverEndPoint = new IPEndPoint(IPAddress.Parse(ipAddress), port); ;

            do
            {
                try
                {
                    _client.Connect(_serverEndPoint);
                    _clientStream = _client.GetStream();
                    return true;
                }
                catch (Exception)
                {
                    if (MessageBox.Show("server is offline, try again?", "error connecting to server", MessageBoxButton.YesNo, MessageBoxImage.Warning) == MessageBoxResult.No)
                        return false;
                }
            } while (true);
        }

        public void mySend(string text)
        {
            byte[] buffer = new ASCIIEncoding().GetBytes(text);
            _clientStream.Write(buffer, 0, buffer.Length);
            _clientStream.Flush();    

            //secure
            //secureStream.Write(buffer);
            //secureStream.Flush();
        }

        public string myReceive(int size)
        {
            byte[] buffer = new byte[size];
   
            int totalBytesRecieved = 0;
            while (totalBytesRecieved < size)
            {
                totalBytesRecieved += _clientStream.Read(buffer, totalBytesRecieved, 1);
            }
            //int count = clientStream.Read(buffer, 0, size);

            //secure
            //secureStream.Read(buffer, 0, size);
            string decoded = new ASCIIEncoding().GetString(buffer); //seperated lines for debugging. TODO (make this a one-liner)
            if (decoded.Contains("\0"))
                throw new Exception("bytes not recieved properly");
            return decoded;            
        }

        public bool isDataAvailable()
        {
            return _clientStream.DataAvailable;
        }
    }
}