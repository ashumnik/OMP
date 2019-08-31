using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using Forms = System.Windows.Forms;
using WPF = System.Windows.Controls;
using System.Net;
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
        string pathToChosenFile = "";
        IPAddress srcIP = null;
        IPAddress destIP= null;

        int srcPort = 0;
        int destPort = 0;
        public MainWindow()
        {
            InitializeComponent();
        }


        private void ChooseFile_Click(object sender, RoutedEventArgs e)
        {

            if (Send.IsChecked.Value)
            {
                var sendFilePath = new Forms.OpenFileDialog();
                sendFilePath.ShowDialog();
                pathToChosenFile = sendFilePath.FileName;
            }
            if (Recieve.IsChecked.Value)
            {
                var sendFilePath = new Forms.SaveFileDialog();
                sendFilePath.ShowDialog();
                pathToChosenFile = sendFilePath.FileName;
            }

            TextBlock_FilePath.Text = pathToChosenFile;
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
            else if (whichSender.Name == "SrcIP")
            {
                srcIP = IP;
            }

            Console.WriteLine($"destIP: {destIP}, srcIP: {srcIP}");

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
            else if (whichSender.Name == "SrcPort")
            {
                srcPort = port;
            }

            Console.WriteLine($"destPort: {destPort}, srcPort: {srcPort}");
        }

        private void Action_Click(object sender, RoutedEventArgs e)
        {

        }
    }
}
