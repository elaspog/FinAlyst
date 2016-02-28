using FinAlyst.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FinAlyst.Infrastructure.Helpers
{
    public class GeneralAnswerData : GeneralParameterData
    {
        public GeneralAnswerData()
        { 

        }
        public GeneralAnswerData(GeneralParameterData generalParameterData)
        {
            Category    = generalParameterData.Category;
            Item        = generalParameterData.Item;
            Plan        = generalParameterData.Plan;
            Data        = generalParameterData.Data;
            Message     = generalParameterData.Message;
            SummaryOfPlansAndExpensesRequest = generalParameterData.SummaryOfPlansAndExpensesRequest;
            SummaryOfDailyCategoryExpensesRequest = generalParameterData.SummaryOfDailyCategoryExpensesRequest;
        }

        private bool success;
        private Item[] items;
        private Category[] categories;
        private Plan[] plans;
        private SummaryOfPlansAndExpensesResult[] summaryOfPlansAndExpensesResults;
        private SummaryOfDailyCategoryExpensesResult[] summaryOfDailyCategoryExpensesResults;

        


        public bool Success
        {
            get { return success; }
            set { success = value; }
        }
        public Item[] Items
        {
            get { return items; }
            set { items = value; }
        }
        public Category[] Categories
        {
            get { return categories; }
            set { categories = value; }
        }
        public Plan[] Plans
        {
            get { return plans; }
            set { plans = value; }
        }
        public SummaryOfPlansAndExpensesResult[] SummaryOfPlansAndExpensesResults
        {
            get { return summaryOfPlansAndExpensesResults; }
            set { summaryOfPlansAndExpensesResults = value; }
        }
        public SummaryOfDailyCategoryExpensesResult[] SummaryOfDailyCategoryExpensesResults
        {
            get { return summaryOfDailyCategoryExpensesResults; }
            set { summaryOfDailyCategoryExpensesResults = value; }
        }

    }
}
