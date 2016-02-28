using FinAlyst.Infrastructure.Helpers;
using FinAlyst.Models;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;

namespace FinAlyst.Infrastructure
{
    // http://stackoverflow.com/questions/14143683/disable-wpf-buttons-during-longer-running-process-the-mvvm-way
    // http://elegantcode.com/2009/07/03/wpf-multithreading-using-the-backgroundworker-and-reporting-the-progress-to-the-ui/
    // http://msdn.microsoft.com/en-us/magazine/cc163328.aspx
    // http://tech.pro/tutorial/800/working-with-the-wpf-dispatcher

    public class RepositoryManager 
    {
        static List<ManagedRequestBase> requestFIFO = new List<ManagedRequestBase>();
        static BackgroundWorker worker = new BackgroundWorker();


        public delegate void Signature_Void_Bool(bool result);
        public delegate void Signature_Void_Void();
        public delegate void Sign_GeneralAnswerData(GeneralAnswerData generalAnswerData);



        public event Signature_Void_Bool RaiseLoginEnded;
        public event Signature_Void_Void RaiseLogoutEnded;

        public event Sign_GeneralAnswerData RaiseItemsQuerried;
        public event Sign_GeneralAnswerData RaisePlansQuerried;
        public event Sign_GeneralAnswerData RaiseCategoriesQuerried;
        public event Sign_GeneralAnswerData RaiseSummariesQuerried;
        public event Sign_GeneralAnswerData RaisePlansAndExpensesSummaryQuerried;
        public event Sign_GeneralAnswerData RaiseDailyCategoryExpensesSummaryQuerried;

        public event Sign_GeneralAnswerData RaiseItemAdded;
        public event Sign_GeneralAnswerData RaiseItemDeleted;
        public event Sign_GeneralAnswerData RaiseItemSaved;

        public event Sign_GeneralAnswerData RaiseCategoryAdded;
        public event Sign_GeneralAnswerData RaiseCategoryDeleted;
        public event Sign_GeneralAnswerData RaiseCategorySaved;

        public event Sign_GeneralAnswerData RaisePlanAdded;
        public event Sign_GeneralAnswerData RaisePlanDeleted;
        public event Sign_GeneralAnswerData RaisePlanSaved;

        

        

        public static IServerConnection SERVICES ;
        public static IOService ioService;


        public RepositoryManager(Logger logger)
        {
            SERVICES = new ServerConnection(logger);
            ioService = new IOService();

            worker.DoWork += (o, ea) =>
            {
                try
                {
                    ManagedRequestBase mr = null;
                    Application.Current.Dispatcher.Invoke(new Action(() => mr = popQueueElement() ));

                    if (mr != null)
                    {
                        processRequestFromQueue(mr);
                        if (requestFIFO != null && requestFIFO.Count > 0)
                            System.Threading.Thread.Sleep(100);
                    }
                }
                catch (Exception)
                {
                }
            };
            
            worker.RunWorkerCompleted += (o, ea) =>
            {
                if (requestFIFO != null && requestFIFO.Count > 0)
                    worker.RunWorkerAsync();
            };

            if ( ! worker.IsBusy )  
                worker.RunWorkerAsync();
        }

        private void processRequestFromQueue(ManagedRequestBase currentlyManagedRequest)
        {
            var @switch = new Dictionary<Type, Action> {
                { typeof(ManagedRequestGeneral<Func<string, string, string, bool>>), 
                    () => {
                        ManagedRequestGeneral<Func<string, string, string, bool>> cmr = (ManagedRequestGeneral<Func<string, string, string, bool>>) currentlyManagedRequest;
                        bool result;
                        switch (currentlyManagedRequest.MethodName.ToString())
                        {
                            case "Login":
                                result = cmr.MethodToCallWithParameters.Invoke("", "", "");
                                RaiseLoginEnded(result);
                                break;
                        }
                    }
                },
                { typeof(ManagedRequestGeneral<Func<GeneralAnswerData>>), 
                    () => {
                        ManagedRequestGeneral<Func<GeneralAnswerData>> cmr = (ManagedRequestGeneral<Func<GeneralAnswerData>>)currentlyManagedRequest;
                        GeneralAnswerData generalAnswerData;
                        switch (currentlyManagedRequest.MethodName.ToString())
                        {
                            case "GetCategories":
                                generalAnswerData = cmr.MethodToCallWithParameters.Invoke();
                                if (RaiseCategoriesQuerried != null)
                                    RaiseCategoriesQuerried(generalAnswerData);
                                break;
                        }
                    } 
                },
                { typeof(ManagedRequestGeneral<Func<GeneralParameterData, GeneralAnswerData>>), 
                    () => {
                        ManagedRequestGeneral<Func<GeneralParameterData, GeneralAnswerData>> cmr = (ManagedRequestGeneral<Func<GeneralParameterData, GeneralAnswerData>>)currentlyManagedRequest;
                        GeneralAnswerData generalAnswerData;
                        switch (currentlyManagedRequest.MethodName.ToString())
                        {
                            case "AddItem":
                                generalAnswerData = cmr.MethodToCallWithParameters.Invoke(null);
                                if (RaiseItemAdded != null)
                                    RaiseItemAdded(generalAnswerData);
                                break;
                            case "AddCategory":
                                generalAnswerData = cmr.MethodToCallWithParameters.Invoke(null);
                                if (RaiseCategoryAdded != null)
                                    RaiseCategoryAdded(generalAnswerData);
                                break;
                            case "AddPlan":
                                generalAnswerData = cmr.MethodToCallWithParameters.Invoke(null);
                                if (RaisePlanAdded != null)
                                    RaisePlanAdded(generalAnswerData);
                                break;

                            case "DeleteItem":
                                generalAnswerData = cmr.MethodToCallWithParameters.Invoke(null);
                                if (RaiseItemDeleted != null)
                                    RaiseItemDeleted(generalAnswerData);
                                break;
                            case "DeleteCategory":
                                generalAnswerData = cmr.MethodToCallWithParameters.Invoke(null);
                                if (RaiseCategoryDeleted != null)
                                    RaiseCategoryDeleted(generalAnswerData);
                                break;
                            case "DeletePlan":
                                generalAnswerData = cmr.MethodToCallWithParameters.Invoke(null);
                                if (RaisePlanDeleted != null)
                                    RaisePlanDeleted(generalAnswerData);
                                break;

                            case "UpdateItem":
                                generalAnswerData = cmr.MethodToCallWithParameters.Invoke(null);
                                if (RaiseItemSaved != null)
                                    RaiseItemSaved(generalAnswerData);
                                break;
                            case "UpdateCategory":
                                generalAnswerData = cmr.MethodToCallWithParameters.Invoke(null);
                                if (RaiseCategorySaved != null)
                                    RaiseCategorySaved(generalAnswerData);
                                break;
                            case "UpdatePlan":
                                generalAnswerData = cmr.MethodToCallWithParameters.Invoke(null);
                                if (RaisePlanSaved != null)
                                    RaisePlanSaved(generalAnswerData);
                                break;

                            case "GetPlansAndExpensesSummary":
                                generalAnswerData = cmr.MethodToCallWithParameters.Invoke(null);
                                if (RaisePlansAndExpensesSummaryQuerried != null)
                                    RaisePlansAndExpensesSummaryQuerried(generalAnswerData);
                                break;
                            case "GetDailyCategoryExpensesSummary":
                                generalAnswerData = cmr.MethodToCallWithParameters.Invoke(null);
                                if (RaiseDailyCategoryExpensesSummaryQuerried != null)
                                    RaiseDailyCategoryExpensesSummaryQuerried(generalAnswerData);
                                break;
                        }
                    } 
                },
                { typeof(ManagedRequestGeneral<Func<bool, bool, bool, GeneralAnswerData>>), 
                    () => {
                        ManagedRequestGeneral<Func<bool, bool, bool, GeneralAnswerData>> cmr = (ManagedRequestGeneral<Func<bool, bool, bool, GeneralAnswerData>>)currentlyManagedRequest;
                        GeneralAnswerData generalAnswerData;
                        switch (currentlyManagedRequest.MethodName.ToString())
                        {
                            case "GetItems":
                                generalAnswerData = cmr.MethodToCallWithParameters.Invoke(true, true, false);
                                if (RaiseItemsQuerried != null)
                                    RaiseItemsQuerried(generalAnswerData);
                                break; 
                            case "GetPlans":
                                generalAnswerData = cmr.MethodToCallWithParameters.Invoke(false,true,true);
                                if (RaisePlansQuerried != null)
                                    RaisePlansQuerried(generalAnswerData);
                                break;
                            case "GetSummaries":
                                generalAnswerData = cmr.MethodToCallWithParameters.Invoke(false, true, false);
                                if (RaiseSummariesQuerried != null)
                                    RaiseSummariesQuerried(generalAnswerData);
                                break;
                        }
                    } 
                },
                { typeof(ManagedRequestGeneral<Action>), 
                    () => {
                        ManagedRequestGeneral<Action> cmr = (ManagedRequestGeneral<Action>)currentlyManagedRequest;
                        switch (currentlyManagedRequest.MethodName.ToString())
                        {
                            case "Logout":
                                cmr.MethodToCallWithParameters.Invoke();
                                RaiseLogoutEnded();
                                break;
                        }                    
                    }                     
                },
            };

            Type type = currentlyManagedRequest.GetType();

            @switch[type]();
        }
        
        private ManagedRequestBase popQueueElement()
        {
            if (requestFIFO != null && requestFIFO.Count > 0)
            {
                ManagedRequestBase result = requestFIFO.ElementAt(0);
                requestFIFO.Remove(result);
                return result;
            }
            else
                return null;
        }

        private static void addAndRunRequestAsPossible<T>(T methodToCall, string methodName)
        {
            addDoSomething<T>(methodToCall, methodName);
            if (!worker.IsBusy)
                worker.RunWorkerAsync();
        }

        private static void addDoSomething<T>(T methodToCall, string methodName)
        {
            requestFIFO.Add(new ManagedRequestGeneral<T>(methodName, methodToCall));
        }




        public void Login(string serverURL, string userName, string password)
        {
            addAndRunRequestAsPossible<Func<string, string, string, bool>>
                ((a, b, c) => SERVICES.Login(serverURL, userName, password), "Login");
        }

        public void Logout()
        {
            addAndRunRequestAsPossible<Action>(() => SERVICES.Logout(), "Logout");
        }



        public void GetItems()
        {
            addAndRunRequestAsPossible<Func<bool, bool, bool, GeneralAnswerData>>
                ((a, b, c) => SERVICES.getListsOfThings(true, true, false), "GetItems"); 
        }

        public void GetPlans()
        {
            addAndRunRequestAsPossible<Func<bool, bool, bool, GeneralAnswerData>>
                ((a, b, c) => SERVICES.getListsOfThings(false, true, true), "GetPlans");  
        }

        public void GetCategories()
        {
            addAndRunRequestAsPossible<Func<GeneralAnswerData>>
                (() => SERVICES.getCategories(), "GetCategories");
        }

        public void GetSummaries()
        {
            addAndRunRequestAsPossible<Func<bool, bool, bool, GeneralAnswerData>>
                ((a, b, c) => SERVICES.getListsOfThings(false, true, false), "GetSummaries");  
        }

        public void GetPlansAndExpensesSummary(GeneralParameterData generalParameterData)
        {
            addAndRunRequestAsPossible<Func<GeneralParameterData, GeneralAnswerData>>
                ((a) => SERVICES.getPlansAndExpensesSummary(generalParameterData), "GetPlansAndExpensesSummary");  
        }

        public void GetDailyCategoryExpensesSummary(GeneralParameterData generalParameterData)
        {
            addAndRunRequestAsPossible<Func<GeneralParameterData, GeneralAnswerData>>
                ((a) => SERVICES.getDailyCategoryExpensesOverview(generalParameterData), "GetDailyCategoryExpensesSummary");  
        }
        

        public void CreateCategory(GeneralParameterData generalParameterData)
        {
            addAndRunRequestAsPossible<Func<GeneralParameterData, GeneralAnswerData>>
                ((a) => SERVICES.createCategory(generalParameterData), "AddCategory");
        }

        public void UpdateCategory(GeneralParameterData generalParameterData)
        {
            addAndRunRequestAsPossible<Func<GeneralParameterData, GeneralAnswerData>>
                ((a) => SERVICES.updateCategory(generalParameterData), "UpdateCategory");
        }

        public void DestoryCategory(GeneralParameterData generalParameterData)
        {
            addAndRunRequestAsPossible<Func<GeneralParameterData, GeneralAnswerData>>
                ((a) => SERVICES.destoryCategory(generalParameterData), "DeleteCategory");
        }

        public void CreateItem(GeneralParameterData generalParameterData)
        {
            addAndRunRequestAsPossible<Func<GeneralParameterData, GeneralAnswerData>>
                ((a) => SERVICES.createItem(generalParameterData), "AddItem");
        }

        public void UpdateItem(GeneralParameterData generalParameterData)
        {
            addAndRunRequestAsPossible<Func<GeneralParameterData, GeneralAnswerData>>
                ((a) => SERVICES.updateItem(generalParameterData), "UpdateItem");
        }

        public void DestoryItem(GeneralParameterData generalParameterData)
        {
            addAndRunRequestAsPossible<Func<GeneralParameterData, GeneralAnswerData>>
                ((a) => SERVICES.destoryItem(generalParameterData), "DeleteItem");
        }


        public void CreatePlan(GeneralParameterData generalParameterData)
        {
            addAndRunRequestAsPossible<Func<GeneralParameterData, GeneralAnswerData>>
                ((a) => SERVICES.createPlan(generalParameterData), "AddPlan");
        }

        public void UpdatePlan(GeneralParameterData generalParameterData)
        {
            addAndRunRequestAsPossible<Func<GeneralParameterData, GeneralAnswerData>>
                ((a) => SERVICES.updatePlan(generalParameterData), "UpdatePlan");
        }

        public void DeletePlan(GeneralParameterData generalParameterData)
        {
            addAndRunRequestAsPossible<Func<GeneralParameterData, GeneralAnswerData>>
                ((a) => SERVICES.destoryPlan(generalParameterData), "DeletePlan");
        }


        public ConfigurationDescriptor loadConfiguration(string fileName, ConfigurationDescriptor configurationDescriptor)
        {
            return ioService.GetConnections(fileName, configurationDescriptor);
        }

        public void writeConfiguration(string fileName, ConfigurationDescriptor configurationDescriptor)
        {
            ioService.WriteConfiguration(fileName, configurationDescriptor);
        }
    }
}
