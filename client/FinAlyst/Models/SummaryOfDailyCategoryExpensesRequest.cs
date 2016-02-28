using FinAlyst.Infrastructure;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace FinAlyst.Models
{
    [DataContract]
    public class SummaryOfDailyCategoryExpensesRequest : Notifier
    {

        [DataMember(Name = "relative_month")]
        private string relativeMonth;
        public string RelativeMonth
        {
            get { return relativeMonth; }
            set
            {
                relativeMonth = value;
                OnPropertyChanged("RelativeMonth");
            }
        }

        private string selectedMonthStr;
        public string SelectedMonthStr
        {
            get { return selectedMonthStr; }
            set
            {
                selectedMonthStr = value;
                OnPropertyChanged("SelectedMonthStr");
            }
        }
    }
}
