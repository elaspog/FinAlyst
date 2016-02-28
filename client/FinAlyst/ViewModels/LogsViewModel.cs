using FinAlyst.Infrastructure;
using GalaSoft.MvvmLight;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FinAlyst.ViewModels
{
    public class LogsViewModel : ViewModelBase
    {
        private Logger logger;
        public Logger Logger
        {
            get { return logger; }
        }

        public LogsViewModel()
        {
        }

        public void setLogger(Logger logger)
        {
            this.logger = logger;
        }
    }
}
