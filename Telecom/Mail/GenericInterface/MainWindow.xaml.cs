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
using System.Net.Mail;
using OpenPop.Pop3;

namespace GenericInterface
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        bool send_occured = false;

        public MainWindow()
        {
            InitializeComponent();
        }

        private void Send_Click(object sender, RoutedEventArgs e)
        {
            // https://www.minuteinbox.com/ -- адрес сайта для проверки прихода сообщения
            // https://dnschecker.org/mx-lookup.php -- сайт для получения IP почтового сервиса

            // отправитель - устанавливаем адрес и отображаемое в письме имя
            MailAddress from = new MailAddress(From.Text);
            // кому отправляем
            MailAddress to = new MailAddress(To.Text);
            // создаем объект сообщения
            MailMessage m = new MailMessage(from, to);
            // текст письма
            m.Body = Mail.Text;
            // адрес smtp-сервера и порт, с которого будем отправлять письмо
            SmtpClient smtp = new SmtpClient("107.161.23.226");
            smtp.Send(m);
            send_occured = true;
        }

        private void Fetch_Click(object sender, RoutedEventArgs e)
        {
            if (send_occured && FromFetch.Text == To.Text)
            {
                var new_message = new WPF.TextBox();
                new_message.Text = From.Text + " | " + Mail.Text;
                StackContainer.Children.Add(new_message);
                send_occured = false;
            }
        }
    }
}
