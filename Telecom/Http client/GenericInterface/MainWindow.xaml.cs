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
                angle += 10;
                Application.Current.Resources["Angle"] = angle;
            };
        }

        private void Load_Click(object sender, RoutedEventArgs e)
        {
            timer.Start();

            HTTPBrowser.Source = new Uri(SrcAddr.Text);
            
            timer.Stop();
        }
        
    }
}
