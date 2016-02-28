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
    // http://vortexwolf.wordpress.com/2011/10/01/wpf-validation-with-idataerrorinfo/

    public class ErrorExtendedViewModelBase : ViewModelBase, IDataErrorInfo
    {
        protected ModelValidator _validator = new ModelValidator();

        public ErrorExtendedViewModelBase()
        {
            //Validates on changes of properties
            this.PropertyChanged += (s, e) =>
            {
                this._validator.ValidateProperty(e.PropertyName);
            };
        }

        public string Error
        {
            get { throw new NotImplementedException(); }
        }

        public string this[string columnName]
        {
            get { return this._validator.GetErrors(columnName).FirstOrDefault(); }
        }

    }
}
