using System;
using System.Collections.Generic;
using System.Linq;
using System.Linq.Expressions;
using System.Reflection;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace FinAlyst.ViewModels.Validation
{
    // http://vortexwolf.wordpress.com/2011/10/01/wpf-validation-with-idataerrorinfo/

    public class PropertyValidation<TP> : IPropertyValidation
    {
        private const string DefaultValidationError = "Validation error";
        private readonly List<Func<bool>> _validationCriterias = new List<Func<bool>>();
        private Func<bool> _validationPrecondition;

        private readonly Func<TP> _getPropertyValue;

        public PropertyValidation(Expression<Func<TP>> expression)
        {
            this._getPropertyValue = expression.Compile();
            this.PropertyName = GetPropertyName(expression);
            this.ErrorMessage = DefaultValidationError;
        }

        public string PropertyName { get; private set; }
        public string ErrorMessage { get; private set; }
        public TP PropertyValue { get { return this._getPropertyValue(); } }

        /// <summary>
        /// Sets the condition whether to validate or not
        /// </summary>
        public PropertyValidation<TP> When(Func<bool> validationPrecondition)
        {
            _validationPrecondition = validationPrecondition;
            return this;
        }

        /// <summary>
        /// Sets the error message when the property is invalid
        /// </summary>
        public PropertyValidation<TP> Show(string errorMessage)
        {
            if (this.ErrorMessage != null && this.ErrorMessage != DefaultValidationError)
                throw new InvalidOperationException("You can set the error message only once.");

            this.ErrorMessage = errorMessage;
            return this;
        }

        /// <summary>
        /// Validates the property using applied criterias
        /// </summary>
        public bool IsInvalid()
        {
            if (_validationCriterias.Count == 0)
                throw new InvalidOperationException("No criteria has been provided for this validation.");

            if (_validationPrecondition != null && !_validationPrecondition())
                return false;

            return _validationCriterias.Any(f => !f());
        }

        /// <summary>
        /// Sets the rule that the property shouldn't be null
        /// </summary>
        public PropertyValidation<TP> NotNull()
        {
            _validationCriterias.Add(CheckIsNotNullValue);
            return this;
        }

        /// <summary>
        /// Sets the rule that the string property shouldn't be null or empty
        /// </summary>
        public PropertyValidation<TP> NotEmpty()
        {
            _validationCriterias.Add(CheckIsNotEmptyValue);
            return this;
        }

        /// <summary>
        /// Sets the rule that the property should be within a specific range
        /// </summary>
        public PropertyValidation<TP> Between<TC>(TC left, TC right) where TC : IComparable<TC>
        {
            if (left != null && right != null)
                _validationCriterias.Add(() => CheckBetween(left, right));
            return this;
        }

        /// <summary>
        /// Sets the rule that the property should contain a valid e-mail address
        /// </summary>
        public PropertyValidation<TP> EmailAddress()
        {
            _validationCriterias.Add(CheckEmailAddress);
            return this;
        }

        /// <summary>
        /// Sets the rule that the property should satisfy custom criteria
        /// </summary>
        public PropertyValidation<TP> Must(Func<bool> validationCriteria)
        {
            _validationCriterias.Add(validationCriteria);
            return this;
        }


        public PropertyValidation<TP> StringLengthBetween(int left, int right) 
        {
            _validationCriterias.Add(() => CheckStringLengthBetween(left, right));
            return this;
        }




        private bool CheckIsNotNullValue()
        {
            return this.PropertyValue != null;
        }

        private bool CheckIsNotEmptyValue()
        {
            object val = this.PropertyValue;

            if (val is string)
                return !string.IsNullOrWhiteSpace((string)val);
            else
                return val != null;
        }

        private bool CheckBetween<TC>(TC left, TC right) where TC : IComparable<TC>
        {
            object val = this.PropertyValue;
            if (val == null)
                return false;

            //Try to parse the string
            if (val is string && left is IConvertible && right is IConvertible)
            {
                double dVal;
                return double.TryParse((string)val, out dVal) &&
                            dVal >= Convert.ToDouble(left) &&
                            dVal <= Convert.ToDouble(right);
            }

            if (val is IConvertible)
            {
                TC tcVal = (TC)Convert.ChangeType(val, typeof(TC));
                return tcVal != null
                    && tcVal.CompareTo(left) >= 0
                    && tcVal.CompareTo(right) <= 0;
            }

            return false;
        }

        private bool CheckEmailAddress()
        {
            string val = this.PropertyValue as string;
            return val == null ||
                    val != null &&
                    (
                        !string.IsNullOrWhiteSpace(val) &&
                        Regex.IsMatch(val, @"^(?:[\w\!\#\$\%\&\'\*\+\-\/\=\?\^\`\{\|\}\~]+\.)*[\w\!\#\$\%\&\'\*\+\-\/\=\?\^\`\{\|\}\~]+@(?:(?:(?:[a-zA-Z0-9](?:[a-zA-Z0-9\-](?!\.)){0,61}[a-zA-Z0-9]?\.)+[a-zA-Z0-9](?:[a-zA-Z0-9\-](?!$)){0,61}[a-zA-Z0-9]?)|(?:\[(?:(?:[01]?\d{1,2}|2[0-4]\d|25[0-5])\.){3}(?:[01]?\d{1,2}|2[0-4]\d|25[0-5])\]))$")
                        ||
                        string.IsNullOrWhiteSpace(val)
                    );
        }

        private bool CheckStringLengthBetween(int left, int right)
        {
            object val = this.PropertyValue;

            if (val is string)
                if ((((string)val).Length >= left) && (((string)val).Length <= right))
                    return true;
            return false;
        }



        private static string GetPropertyName(Expression<Func<TP>> expression)
        {
            if (expression == null)
                throw new ArgumentNullException("expression");

            MemberExpression memberExpression;

            if (expression.Body is UnaryExpression)
                memberExpression = ((UnaryExpression)expression.Body).Operand as MemberExpression;
            else
                memberExpression = expression.Body as MemberExpression;

            if (memberExpression == null)
                throw new ArgumentException("The expression is not a member access expression", "expression");

            var property = memberExpression.Member as PropertyInfo;
            if (property == null)
                throw new ArgumentException("The member access expression does not access a property", "expression");

            var getMethod = property.GetGetMethod(true);
            if (getMethod.IsStatic)
                throw new ArgumentException("The referenced property is a static property", "expression");
            return memberExpression.Member.Name;

        }

    }

}
