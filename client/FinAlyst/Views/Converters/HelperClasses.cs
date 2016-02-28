using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FinAlyst.Views.Converters
{
    public class ColumnConfig
    {
        public ColumnConfig()
        {
            Columns = new List<Column>();
        }

        public List<Column> Columns { get; set; }
    }

    public class Column
    {
        public string Header { get; set; }
        public string DataField { get; set; }
    }

}
