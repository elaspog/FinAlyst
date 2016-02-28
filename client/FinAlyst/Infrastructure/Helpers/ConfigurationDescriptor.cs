using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Serialization;

namespace FinAlyst.Infrastructure.Helpers
{
    [Serializable]
    [XmlRootAttribute(ElementName = "Connections")]
    public class ConfigurationDescriptor : Notifier
    {

        public ConfigurationDescriptor()
        { 
        
        }

        public ConfigurationDescriptor(ObservableCollection<string> testList)
        {
            // TODO: Complete member initialization
            this.connection = testList;
        }

        private ObservableCollection<string> connection;
        [XmlArrayItem("Connection", Type = typeof(string))]
        public ObservableCollection<string> Connection
        {
            get { return connection; }
            set
            {
                connection = value;
                OnPropertyChanged("Connection");
            }
        }

    }
}
