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

namespace FinAlyst.Views.CustomControls
{
    /// <summary>
    /// Follow steps 1a or 1b and then 2 to use this custom control in a XAML file.
    ///
    /// Step 1a) Using this custom control in a XAML file that exists in the current project.
    /// Add this XmlNamespace attribute to the root element of the markup file where it is 
    /// to be used:
    ///
    ///     xmlns:MyNamespace="clr-namespace:FinAlyst.Views.CustomControls"
    ///
    ///
    /// Step 1b) Using this custom control in a XAML file that exists in a different project.
    /// Add this XmlNamespace attribute to the root element of the markup file where it is 
    /// to be used:
    ///
    ///     xmlns:MyNamespace="clr-namespace:FinAlyst.Views.CustomControls;assembly=FinAlyst.Views.CustomControls"
    ///
    /// You will also need to add a project reference from the project where the XAML file lives
    /// to this project and Rebuild to avoid compilation errors:
    ///
    ///     Right click on the target project in the Solution Explorer and
    ///     "Add Reference"->"Projects"->[Browse to and select this project]
    ///
    ///
    /// Step 2)
    /// Go ahead and use your control in the XAML file.
    ///
    ///     <MyNamespace:AnimatedExpander/>
    ///
    /// </summary>
    public class AnimatedExpander : ContentControl
    {
        static AnimatedExpander()
        {
            DefaultStyleKeyProperty.OverrideMetadata(typeof(AnimatedExpander), new FrameworkPropertyMetadata(typeof(AnimatedExpander)));
        }

        public string HeaderName
        {
            get { return (string)this.GetValue(HeaderNameProperty); }
            set { this.SetValue(HeaderNameProperty, value); }
        }
        public static readonly DependencyProperty HeaderNameProperty = DependencyProperty.Register(
          "HeaderName", typeof(string), typeof(AnimatedExpander), new PropertyMetadata(string.Empty));


        public override void OnApplyTemplate()
        {
            base.OnApplyTemplate();

            if (this.Template != null)
            {
                var expander = (Expander)Template.FindName("animatedExpander", this);
                if (expander != null)
                    expander.Collapsed += new RoutedEventHandler(exp3_Collapsed);
            }
        }

        private void exp3_Collapsed(object sender, RoutedEventArgs e)
        {
            var expander = (Expander)Template.FindName("animatedExpander", this);
            var expanderUI = expander as UIElement;
            expanderUI.Visibility = System.Windows.Visibility.Visible;

            var storyboardCollapse = (Storyboard)expander.FindResource("storyboardCollapse");
            storyboardCollapse.Begin();
        }

        public void openExpander()
        {
            Expander expander = (Expander)Template.FindName("animatedExpander", this);
            expander.IsExpanded = true;
        }


        public void closeExpander()
        {
            Expander expander = (Expander)Template.FindName("animatedExpander", this);
            expander.IsExpanded = false;
        }

    }
}
