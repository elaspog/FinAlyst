using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FinAlyst.Infrastructure.Helpers
{
    abstract class ManagedRequestBase
    {
        private string methodName;
        public string MethodName
        {
            get { return methodName; }
            set { methodName = value; }
        }        

        public ManagedRequestBase()
        {
        }

        public ManagedRequestBase(string methodName)
        {
            this.methodName = methodName;
        }

        //public string ExtractMethodName(Func<string, string, string, bool> func)
        //{
        //    var il = func.Method.GetMethodBody().GetILAsByteArray();

        //    // first byte is ldarg.0
        //    // second byte is callvirt
        //    // next four bytes are the MethodDef token
        //    var mdToken = (il[5] << 24) | (il[4] << 16) | (il[3] << 8) | il[2];
        //    var innerMethod = func.Method.Module.ResolveMethod(mdToken);

        //    // Check to see if this is a property getter and grab property if it is...
        //    if (innerMethod.IsSpecialName && innerMethod.Name.StartsWith("get_"))
        //    {
        //        var prop = (from p in innerMethod.DeclaringType.GetProperties()
        //                    where p.GetGetMethod() == innerMethod
        //                    select p).FirstOrDefault();
        //        if (prop != null)
        //            return prop.Name;
        //    }

        //    return innerMethod.Name;
        //}

    }
}
