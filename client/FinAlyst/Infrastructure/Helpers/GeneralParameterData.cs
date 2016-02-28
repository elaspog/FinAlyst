using FinAlyst.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FinAlyst.Infrastructure.Helpers
{
    public class GeneralParameterData
    {
        public GeneralParameterData()
        {

        }

        protected string message;
        protected string[] data;
        protected Item item;
        protected Category category;
        protected Plan plan;
        protected SummaryOfPlansAndExpensesRequest summaryOfPlansAndExpensesRequest;
        protected SummaryOfDailyCategoryExpensesRequest summaryOfDailyCategoryExpensesRequest;

        public string Message
        {
            get { return message; }
            set { message = value; }
        }
        public string[] Data
        {
            get { return data; }
            set { data = value; }
        }
        public Item Item
        {
            get { return item; }
            set { item = value; }
        }
        public Category Category
        {
            get { return category; }
            set { category = value; }
        }
        public Plan Plan
        {
            get { return plan; }
            set { plan = value; }
        }
        public SummaryOfPlansAndExpensesRequest SummaryOfPlansAndExpensesRequest
        {
            get { return summaryOfPlansAndExpensesRequest; }
            set { summaryOfPlansAndExpensesRequest = value; }
        }
        public SummaryOfDailyCategoryExpensesRequest SummaryOfDailyCategoryExpensesRequest
        {
            get { return summaryOfDailyCategoryExpensesRequest; }
            set { summaryOfDailyCategoryExpensesRequest = value; }
        }        


    }
}
