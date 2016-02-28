using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FinAlyst.Infrastructure.Helpers
{
    class ManagedRequestGeneral<T> : ManagedRequestBase
    {
        private T methodToCallWithParameters;
        
        public ManagedRequestGeneral()
        {
        }

        public ManagedRequestGeneral(string methodName, T methodToCallWithParameters)
            : base(methodName)
        {
            this.methodToCallWithParameters = methodToCallWithParameters;
        }

        public T MethodToCallWithParameters
        {
            get { return methodToCallWithParameters; }
            set { methodToCallWithParameters = value; }
        }
    }
}
