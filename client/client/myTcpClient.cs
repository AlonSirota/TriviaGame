using System.Text;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System;
using System.Windows;
using System.Net.Security;
using System.Security.Cryptography.X509Certificates;

namespace client
{
    public class myTcpClient
    {
        TcpClient _client;
        IPEndPoint _serverEndPoint;
        NetworkStream _clientStream;
        bool _secured;
        SslStream _secureStream;
        //X509Certificate _serverCertificate;

        public myTcpClient(bool secured)
        {
            _client = new TcpClient();
            _serverEndPoint = null;
            _clientStream = null;
            _secured = secured;
            _secureStream = null;
            //_client.Close();
        }

        public bool connect(string ipAddress, int port)
        {
            _serverEndPoint = new IPEndPoint(IPAddress.Parse(ipAddress), port);

            do
            {
                try
                {
                    _client.Connect(_serverEndPoint);

                    if (_secured)
                    {
                        _secureStream = new SslStream(_client.GetStream());
                    }
                    else
                    {
                        _clientStream = _client.GetStream();
                    }                                          
                    
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

            try
            {
                if (_secured)
                {
                    _secureStream.Write(buffer, 0, buffer.Length);
                    _secureStream.Flush();
                }
                else
                {
                    _clientStream.Write(buffer, 0, buffer.Length);
                    _clientStream.Flush();
                }
            }
            catch (Exception e)
            {
                string errorMessage = "server closed unexpectedly";
                MessageBox.Show(errorMessage);
                throw new Exception(errorMessage);
            }                  
        }

        public string myReceive(int size)
        {
            byte[] buffer = new byte[size];
   
            int totalBytesRecieved = 0;
            while (totalBytesRecieved < size)
            {
                try
                {
                    if (_secured)
                        totalBytesRecieved += _secureStream.Read(buffer, totalBytesRecieved, size - totalBytesRecieved);
                    else
                        totalBytesRecieved += _clientStream.Read(buffer, totalBytesRecieved, size - totalBytesRecieved);
                }
                catch(Exception)
                { 
                    string errorMessage = "server closed unexpectedly";
                    MessageBox.Show(errorMessage);
                    throw new Exception(errorMessage);
                }                
            }

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