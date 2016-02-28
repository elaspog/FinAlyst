using FinAlyst.Infrastructure.Helpers;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FinAlyst.Infrastructure
{
    public interface IServerConnection
    {

        bool Login(string serverURL, string userName, string password);

        bool isLoggedIn();

        bool IsNotLoggedIn();

        bool isConnected();
        
        void Logout();


        GeneralAnswerData getItems();

        GeneralAnswerData getPlans();

        GeneralAnswerData getCategories();

        GeneralAnswerData getListsOfThings(bool getItems, bool getCategories, bool getPlans);

        
        GeneralAnswerData createItem(GeneralParameterData toCreate);

        GeneralAnswerData createCategory(GeneralParameterData toCreate);

        GeneralAnswerData createPlan(GeneralParameterData toCreate);
        

        GeneralAnswerData destoryItem(GeneralParameterData toDestroy);

        GeneralAnswerData destoryCategory(GeneralParameterData toDestroy);

        GeneralAnswerData destoryPlan(GeneralParameterData toDestroy);


        GeneralAnswerData updateItem(GeneralParameterData toUpdate);

        GeneralAnswerData updateCategory(GeneralParameterData toUpdate);

        GeneralAnswerData updatePlan(GeneralParameterData toUpdate);


        GeneralAnswerData getPlansAndExpensesSummary(GeneralParameterData generalParameterData);

        GeneralAnswerData getDailyCategoryExpensesOverview(GeneralParameterData generalParameterData);

        
    }
}
