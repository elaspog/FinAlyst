using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;
using System;
using FinAlyst.Infrastructure;

namespace FinAlyst.Models
{
    [DataContract]
    public class Category : Notifier
    {
        [DataMember(Name = "id")]
        private int id;
        public int Id
        {
            get { return id; }
            set 
            { 
                id = value; 
                OnPropertyChanged("Id"); 
            }
        }

        [DataMember(Name = "name")]
        private string name;
        public string CategoryName
        {
            get { return name; }
            set
            {
                name = value;
                OnPropertyChanged("Name");
            }
        }

        [DataMember(Name = "description")]
        private string description;
        public string Description
        {
            get { return description; }
            set
            {
                description = value;
                OnPropertyChanged("Description");
            }
        }

        [DataMember(Name = "create")]
        private DateTime creationTime;
        public DateTime CreationTime
        {
            get { return creationTime; }
            set
            {
                creationTime = value;
                OnPropertyChanged("CreationTime");
            }
        }

        [DataMember(Name = "modify")]
        private DateTime modificationTime;
        public DateTime ModificationTime
        {
            get { return modificationTime; }
            set
            {
                modificationTime = value;
                OnPropertyChanged("ModificationTime");
            }
        }

    }
}
