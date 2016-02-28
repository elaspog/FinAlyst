using FinAlyst.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FinAlyst.ViewModels.ViewModelEntities
{
    public class ItemViewModel : ErrorExtendedViewModelBase
    {
        public ItemViewModel(Item item) //: base()
        {
            this.item = item;

            // validation rules
            this._validator.AddValidationFor(() => this.Description).NotEmpty().Show("Enter a Description");
            this._validator.AddValidationFor(() => this.Description).NotNull().Show("Enter a Description");
            this._validator.AddValidationFor(() => this.Description).StringLengthBetween(1, 10).Show("The length must be between 1 and 127");


        }



        private Item item;
        public Item Item
        {
            get { return item; }
            set
            {
                item = value;
                RaisePropertyChanged("Item");
            }
        }



            public int Id
            {
                get { return Item.Id; }
                set
                {
                    Item.Id = value;
                }
            }

            public int CategoryId
            {
                get { return Item.CategoryId; }
                set
                {
                    Item.CategoryId = value;
                    RaisePropertyChanged("CategoryId");
                }
            }

            public int Amount
            {
                get { return Item.Amount; }
                set
                {
                    Item.Amount = value;
                    RaisePropertyChanged("Amount");
                }
            }

            public string Description
            {
                get { return Item.Description; }
                set
                {
                    Item.Description = value;
                    RaisePropertyChanged("Description");
                }
            }

            public DateTime CreationTime
            {
                get { return Item.CreationTime; }
                set
                {
                    Item.CreationTime = value;
                }
            }

            public DateTime ModificationTime
            {
                get { return Item.ModificationTime; }
                set
                {
                    Item.ModificationTime = value;
                }
            }
    }
}
