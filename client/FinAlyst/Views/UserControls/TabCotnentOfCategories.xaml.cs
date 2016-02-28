using FinAlyst.Models;
using FinAlyst.ViewModels;
using FinAlyst.ViewModels.ViewModelEntities;
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

namespace FinAlyst.Views.UserControls
{
    /// <summary>
    /// Interaction logic for TabCotnentOfCategories.xaml
    /// </summary>
    public partial class TabCotnentOfCategories : UserControl
    {
        public TabCotnentOfCategories()
        {
            InitializeComponent();
            dataGridOfCategories.Visibility = System.Windows.Visibility.Collapsed;
        }

        private void EditCategory_Click(object sender, RoutedEventArgs e)
        {
            EditCategoryContentControl.openExpander();
        }


        bool fancyElementVisible = true;

        private void GroupBox_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (fancyElementVisible)
            {
                dataGridOfCategories.Visibility = System.Windows.Visibility.Visible;
                itemsControlOfCategories.Visibility = System.Windows.Visibility.Collapsed;
                fancyElementVisible = false;
            }
            else
            {
                dataGridOfCategories.Visibility = System.Windows.Visibility.Collapsed;
                itemsControlOfCategories.Visibility = System.Windows.Visibility.Visible;
                fancyElementVisible = true;
            }
        }
    }
}
