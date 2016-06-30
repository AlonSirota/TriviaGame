using System.Text;
using System.Net;
using System.Net.Sockets;
using System.IO;
using System;

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
   
            int totalBytesRecieved = 0;
            while (totalBytesRecieved < size)
            {
                totalBytesRecieved += clientStream.Read(buffer, totalBytesRecieved, 1);
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
            return clientStream.DataAvailable;
        }
    }
}