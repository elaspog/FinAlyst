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
    /// Interaction logic for TabCotnentOfPlans.xaml
    /// </summary>
    public partial class TabCotnentOfPlans : UserControl
    {
        public TabCotnentOfPlans()
        {
            InitializeComponent();

            dataGridOfPlans.Visibility = System.Windows.Visibility.Collapsed;
        }

        private void EditPlan_Click(object sender, RoutedEventArgs e)
        {
            EditPlanContentControl.openExpander();
        }

        bool fancyElementVisible = true;

        private void GroupBox_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            if (fancyElementVisible)
            {
                dataGridOfPlans.Visibility = System.Windows.Visibility.Visible;
                itemsControlOfPlans.Visibility = System.Windows.Visibility.Collapsed;
                fancyElementVisible = false;
            }
            else
            {
                dataGridOfPlans.Visibility = System.Windows.Visibility.Collapsed;
                itemsControlOfPlans.Visibility = System.Windows.Visibility.Visible;
                fancyElementVisible = true;
            }
        }
    }
}
