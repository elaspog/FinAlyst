using FinAlyst.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FinAlyst.ViewModels.ViewModelEntities
{
    public class PlanViewModel : ErrorExtendedViewModelBase
    {
        public PlanViewModel(Plan Plan)
        {
            this.Plan = Plan;

            // validation rules
            this._validator.AddValidationFor(() => this.Description).NotEmpty().Show("Enter a Description");
            this._validator.AddValidationFor(() => this.Description).NotNull().Show("Enter a Description");
            this._validator.AddValidationFor(() => this.Description).StringLengthBetween(1, 10).Show("The length must be between 1 and 127");
        }




        private Plan plan;
        public Plan Plan
        {
            get { return plan; }
            set
            {
                plan = value;
                RaisePropertyChanged("Plan");
            }
        }



            public int Id
            {
                get { return Plan.Id; }
                set
                {
                    Plan.Id = value;
                    RaisePropertyChanged("Id");
                }
            }

            public int CategoryId
            {
                get { return Plan.CategoryId; }
                set
                {
                    Plan.CategoryId = value;
                    RaisePropertyChanged("CategoryId");
                }
            }

            public int Amount
            {
                get { return Plan.Amount; }
                set
                {
                    Plan.Amount = value;
                    RaisePropertyChanged("Amount");
                }
            }

            public string Description
            {
                get { return Plan.Description; }
                set
                {
                    Plan.Description = value;
                    RaisePropertyChanged("Description");
                }
            }

            public DateTime CreationTime
            {
                get { return Plan.CreationTime; }
                set
                {
                    Plan.CreationTime = value;
                    RaisePropertyChanged("CreationTime");
                }
            }

            public DateTime ModificationTime
            {
                get { return Plan.ModificationTime; }
                set
                {
                    Plan.ModificationTime = value;
                    RaisePropertyChanged("ModificationTime");
                }
            }

    }
}
