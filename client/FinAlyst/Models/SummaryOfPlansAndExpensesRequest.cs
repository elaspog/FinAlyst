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
    public class SummaryOfPlansAndExpensesRequest : Notifier
    {
        [DataMember(Name = "categoryid")]
        private string categoryid;
        public string Categoryid
        {
            get { return categoryid; }
            set
            {
                categoryid = value;
                OnPropertyChanged("Categoryid");
            }
        }

        [DataMember(Name = "granulation")]
        private string granulation;
        public string Granulation
        {
            get { return granulation; }
            set
            {
                granulation = value;
                OnPropertyChanged("Granulation");
            }
        }

        [DataMember(Name = "year")]
        private string year;
        public string Year
        {
            get { return year; }
            set
            {
                year = value;
                OnPropertyChanged("Year");
            }
        }
    }
}
