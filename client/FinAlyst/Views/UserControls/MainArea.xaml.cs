using FinAlyst.ViewModels;
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
using System.Windows.Media.Animation;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace FinAlyst.Views.UserControls
{
    /// <summary>
    /// Interaction logic for MainArea.xaml
    /// </summary>
    public partial class MainArea : UserControl
    {
        public MainArea()
        {
            InitializeComponent();
        }

        private void TabControl_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            try
            {
                //Console.WriteLine(e.OriginalSource.GetType());

                if (e.OriginalSource.GetType() == typeof(System.Windows.Controls.TabControl) 
                    && ((System.Windows.Controls.TabControl)sender).Name.Equals("MainTab"))
                {
                    MainViewModel MainViewModel = this.DataContext as MainViewModel;
                    TabItem item = (TabItem)MainTab.SelectedItem;

                    if (item != null)
                    {
                        switch (item.Name)
                        {
                            case "Expenses":
                                MainViewModel.SelectedSubViewToUpdate = Infrastructure.SelectedSubViewToUpdateEnum.EXPENSES_VIEW;
                                break;
                            case "Plans":
                                MainViewModel.SelectedSubViewToUpdate = Infrastructure.SelectedSubViewToUpdateEnum.PLANS_VIEW;
                                break;
                            case "Categories":
                                MainViewModel.SelectedSubViewToUpdate = Infrastructure.SelectedSubViewToUpdateEnum.CATEGORIES_VIEW;
                                break;
                            case "Summary":
                                MainViewModel.SelectedSubViewToUpdate = Infrastructure.SelectedSubViewToUpdateEnum.SUMMARY_VIEW;
                                break;
                            case "Log":
                                break;
                        }
                    }
                }
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show( ex.ToString(), "ERROR");
            }
        }
    }
}
