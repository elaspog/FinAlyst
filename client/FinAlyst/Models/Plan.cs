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
    public class Plan : Notifier
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

        [DataMember(Name = "categoryid")]
        private int categoryId;
        public int CategoryId
        {
            get { return categoryId; }
            set
            {
                categoryId = value;
                OnPropertyChanged("CategoryId");
            }
        }

        [DataMember(Name = "amount")]
        private int amount;
        public int Amount
        {
            get { return amount; }
            set
            {
                amount = value;
                OnPropertyChanged("Amount");
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
