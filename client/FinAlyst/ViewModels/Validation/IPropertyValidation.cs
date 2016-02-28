using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FinAlyst.ViewModels.Validation
{
    // http://vortexwolf.wordpress.com/2011/10/01/wpf-validation-with-idataerrorinfo/

    public interface IPropertyValidation
    {
        string PropertyName { get; }
        string ErrorMessage { get; }
        bool IsInvalid();
    }
}
