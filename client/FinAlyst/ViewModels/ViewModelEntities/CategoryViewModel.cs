using FinAlyst.Models;
using FinAlyst.ViewModels.Validation;
using GalaSoft.MvvmLight;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FinAlyst.ViewModels.ViewModelEntities
{
    public class CategoryViewModel : ErrorExtendedViewModelBase
    {
        

        public CategoryViewModel(Category category) //: base()
        {
            this.category = category;

            // validation rules
            this._validator.AddValidationFor(() => this.CategoryName).NotEmpty().Show("Enter a Category Name");
            this._validator.AddValidationFor(() => this.CategoryName).NotNull().Show("Enter a Category Name");
            this._validator.AddValidationFor(() => this.CategoryName).StringLengthBetween(1, 127).Show("The length must be between 1 and 127");

            this._validator.AddValidationFor(() => this.Description).StringLengthBetween(0, 255).Show("The length must be between 0 and 255");

            //this._validator.AddValidationFor(() => this.FirstName).NotEmpty().Show("Enter a First Name");
            //this._validator.AddValidationFor(() => this.LastName).NotEmpty().Show("Enter a Last Name");
            //this._validator.AddValidationFor(() => this.Age).NotEmpty().Show("Enter an age");
            //this._validator.AddValidationFor(() => this.Age).Between(0,99).Show("The age must be between 0 and 99");
            //this._validator.AddValidationFor(() => this.Email).When(() => this.IsSubscribe).NotEmpty().Show("Enter an email address");
            //this._validator.AddValidationFor(() => this.Email).When(() => this.IsSubscribe).EmailAddress().Show("Enter a valid email address");


            ////Validates on changes of properties
            //this.PropertyChanged += (s, e) =>
            //{
            //    this._validator.ValidateProperty(e.PropertyName);
            //    //if (e.PropertyName == "IsSubscribe" && this.Email != null)
            //    //{
            //    //    this.RaisePropertyChanged("Email");
            //    //}
            //};

        }




        private Category category;
        public Category Category
        {
            get { return category; }
            set {
                Category = value; 
                RaisePropertyChanged("Category"); 
            }
        }



            public int Id
            {
                get { return Category.Id; }
                set { Category.Id = value; }
            }

            public string CategoryName
            {
                get { return Category.CategoryName; }
                set
                {
                    Category.CategoryName = value;
                    RaisePropertyChanged("CategoryName");
                }
            }

            public string Description
            {
                get { return Category.Description; }
                set
                {
                    Category.Description = value;
                    RaisePropertyChanged("Description");
                }
            }

            public DateTime CreationTime
            {
                get { return Category.CreationTime; }
                set { Category.CreationTime = value; }
            }

            public DateTime ModificationTime
            {
                get { return Category.ModificationTime; }
                set { Category.ModificationTime = value; }
            }

    }
}
