using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FinAlyst.Infrastructure
{
    public class Logger : Notifier
    {
        private ObservableCollection<string> logEntries;
        public ObservableCollection<string> LogEntries
        {
            set 
            { 
                logEntries = value;
                OnPropertyChanged("LogEntries");
                OnPropertyChanged("LogEntriesString");
            }
        }
        public string LogEntriesString
        {
            get
            {
                string log = "";
                foreach (string entry in logEntries)
                {
                    log += entry + "\n";
                }
                return log;
            }
        }

        public Logger()
        {
            logEntries = new ObservableCollection<string>();
        }

        public void addEntry(string entry)
        {
            logEntries.Add(entry);
            OnPropertyChanged("LogEntriesString");
        }

        public void addEntryWithTimestamp(string entry)
        {
            string timeStr = DateTime.Now.ToString("HH:mm:ss");
            logEntries.Add("[" +timeStr + "] " + entry);
            OnPropertyChanged("LogEntriesString");
        }
    }
}
