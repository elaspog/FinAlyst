using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Text;
using System.Threading.Tasks;

namespace FinAlyst.ViewModels.Validation
{
    // http://vortexwolf.wordpress.com/2011/10/01/wpf-validation-with-idataerrorinfo/

    public class ModelValidator
    {
        private readonly List<IPropertyValidation> _validations = new List<IPropertyValidation>();

        public ModelValidator()
        {
            this.ErrorMessages = new Dictionary<string, List<string>>();
        }

        public Dictionary<string, List<string>> ErrorMessages { get; private set; }

        public IEnumerable<string> PropertyNames
        {
            get { return _validations.Select(v => v.PropertyName); }
        }

        /// <summary>
        /// Gets the list of errors for a property
        /// </summary>
        public IEnumerable<string> GetErrors(string propertyName)
        {
            return this.ErrorMessages.Where(er => er.Key == propertyName).SelectMany(er => er.Value);
        }

        /// <summary>
        /// Adds a validation to a property
        /// </summary>
        /// <param name="expression">For example, () => this.SelectedCountry</param>
        /// <returns>The property validation for futher customization</returns>
        public PropertyValidation<TP> AddValidationFor<TP>(Expression<Func<TP>> expression)
        {
            var validation = new PropertyValidation<TP>(expression);
            _validations.Add(validation);
            return validation;
        }

        /// <summary>
        /// Validates a specific property of the model
        /// </summary>
        public void ValidateProperty(string propertyName)
        {
            if (!this.PropertyNames.Contains(propertyName))
                return;

            ErrorMessages.Remove(propertyName);
            _validations.Where(v => v.PropertyName == propertyName).ToList().ForEach(PerformValidation);
        }

        /// <summary>
        /// Validates all of the properties of the model
        /// </summary>
        public bool ValidateAll()
        {
            ErrorMessages.Clear();
            _validations.ForEach(PerformValidation);
            return !ErrorMessages.Any();
        }

        /// <summary>
        /// Validates a property and adds errors to the list if the property is invalid
        /// </summary>
        private void PerformValidation(IPropertyValidation validation)
        {
            if (validation.IsInvalid())
                AddErrorMessageForProperty(validation.PropertyName, validation.ErrorMessage);
        }

        /// <summary>
        /// Adds an error to the dictionary of properties and errors
        /// </summary>
        private void AddErrorMessageForProperty(string propertyName, string errorMessage)
        {
            if (ErrorMessages.ContainsKey(propertyName))
                ErrorMessages[propertyName].Add(errorMessage);
            else
                ErrorMessages.Add(propertyName, new List<string> { errorMessage });
        }


    }
}
