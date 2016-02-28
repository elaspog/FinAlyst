using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FinAlyst.Infrastructure.MessageArgs
{
    public class SendModalWindowMessage
    {
        private string header;
        private string content;

        public SendModalWindowMessage(string _header, string _content)
        {
            header = _header;
            content = _content;
        }

        public string Header
        {
            get { return header; }
            set { header = value; }
        }

        public string Content
        {
            get { return content; }
            set { content = value; }
        }
    }
}
