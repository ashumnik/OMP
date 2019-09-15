using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using Forms = System.Windows.Forms;
using WPF = System.Windows.Controls;
using System.Net;
using System.Net.Sockets;
using System.Timers;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace GenericInterface
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        IPAddress destIP= null;
        Timer timer = new Timer(10);

        int destPort = 0;
        public MainWindow()
        {
            InitializeComponent();

            timer.Elapsed += (a, b) =>
            {
                var angle = (Double)Application.Current.Resources["Angle"];
                angle += 5;
                Application.Current.Resources["Angle"] = angle;
            };
        }
        private void IP_LostFocus(object sender, RoutedEventArgs e)
        {
            IPAddress IP;
            var whichSender = sender as WPF.TextBox;

            if (!IPAddress.TryParse(whichSender.Text, out IP))
            {
                Forms.MessageBox.Show("Вы ввели некорректный IP");
            }
            if(whichSender.Name == "DestIP")
            {
                destIP= IP;
            }

        }
        private void Port_LostFocus(object sender, RoutedEventArgs e)
        {
            int port;
            var whichSender = sender as WPF.TextBox;

            if (!Int32.TryParse(whichSender.Text, out port))
            {
                Forms.MessageBox.Show("Вы ввели некорректный порт");
            }

            if (whichSender.Name == "DestPort")
            {
                destPort = port;
            }
        }

        private void ParsePacket()
        {


        }

        private void Listen_Click(object sender, RoutedEventArgs e)
        {
            timer.Start();
            Socket socket = new Socket(destIP.AddressFamily,SocketType.Raw, ProtocolType.Raw);
            socket.Listen(1);
            byte[] recieved_bytes = new byte[256];
            socket.Receive(recieved_bytes);

            timer.Stop();
        }
    }
}
