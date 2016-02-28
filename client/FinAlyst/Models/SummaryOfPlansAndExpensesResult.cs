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
    public class SummaryOfPlansAndExpensesResult : Notifier
    {
        [DataMember(Name = "interval")]
        private int interval;
        public int Interval
        {
            get { return interval; }
            set
            {
                interval = value;
                OnPropertyChanged("Interval");
            }
        }

        [DataMember(Name = "expensesum")]
        private int expenseSum;
        public int ExpenseSum
        {
            get { return expenseSum; }
            set
            {
                expenseSum = value;
                OnPropertyChanged("ExpenseSum");
            }
        }

        [DataMember(Name = "plannedsum")]
        private int plannedSum;
        public int PlannedSum
        {
            get { return plannedSum; }
            set
            {
                plannedSum = value;
                OnPropertyChanged("PlannedSum");
            }
        }

        [DataMember(Name = "cumulative")]
        private int cumulative;
        public int Cumulative
        {
            get { return cumulative; }
            set
            {
                cumulative = value;
                OnPropertyChanged("Cumulative");
            }
        }

    }
}
