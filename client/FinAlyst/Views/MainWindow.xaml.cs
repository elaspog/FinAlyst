using FinAlyst.Infrastructure.MessageArgs;
using FinAlyst.Views.CustomControls;
using FinAlyst.Views.UserControls;
using GalaSoft.MvvmLight.Messaging;
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

namespace FinAlyst.Views
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();


            Messenger.Default.Register<SendModalWindowMessage>(this, message =>
            {
                System.Windows.MessageBox.Show(((SendModalWindowMessage)message).Header, ((SendModalWindowMessage)message).Content);
            });


            Messenger.Default.Register<OpenAboutWindowMessage>(this, message =>
            {
                openAboutWindow();
            });

            Messenger.Default.Register<OpenConnectionSelectorWindowMessage>(this, message =>
            {
                openConnectionSelectorWindow();
            });

            
        }

        private void openConnectionSelectorWindow()
        {
            ConnectionSelector connectionselectorWindow = new ConnectionSelector();

            connectionselectorWindow.Show();
        }


        private void openAboutWindow()
        {
            AboutWindow aboutWindow = new AboutWindow();

            aboutWindow.Show();
        }

    }
}
