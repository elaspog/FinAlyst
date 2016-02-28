using FinAlyst.Models;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Net;
using System.Runtime.Serialization.Json;
using System.Text;
using System.Threading.Tasks;
using System;
using GalaSoft.MvvmLight.Messaging;
using FinAlyst.Infrastructure.MessageArgs;
using FinAlyst.Infrastructure.Exceptions;
using FinAlyst.ViewModels;
using System.Threading;
using FinAlyst.Infrastructure.Helpers;

namespace FinAlyst.Infrastructure
{
    public class ServerConnection : Notifier, IServerConnection
    {
        Logger logger;

        public ServerConnection(Logger logger)
        {
            this.logger = logger;
        }

        private string hostName;
        public string HostName
        {
            get { return hostName; }
            private set 
            {
                hostName = value;
                OnPropertyChanged("HostName");
            }
        }

        private string userName;
        public string UserName
        {
            get { return userName; }
            private set
            {
                userName = value;
                OnPropertyChanged("UserName");
            }
        }

        private string password;
        public string Password
        {
            get { return password; }
            private set
            {
                password = value;
                OnPropertyChanged("Password");
            }
        }

        private string sessionId;
        public string SessionId
        {
            get { return sessionId; }
            private set
            {
                sessionId = value;
                OnPropertyChanged("SessionId");
            }
        }



        Stream GenerateStreamFromString(string s)
        {
            MemoryStream stream = new MemoryStream();
            StreamWriter writer = new StreamWriter(stream);
            writer.Write(s);
            writer.Flush();
            stream.Position = 0;
            return stream;
        }



        public bool Login(string serverURL, string userName, string password)
        {
            Console.WriteLine("Login");
            if (!String.IsNullOrEmpty(serverURL) && !String.IsNullOrEmpty(userName) && !String.IsNullOrEmpty(password))
            {
                try
                {
                    Stream dataStream;
                    WebRequest request;
                    string datastr;
                    UTF8Encoding encoding = new UTF8Encoding();

                    request = WebRequest.Create( serverURL + "?q=webservice/login");
                    request.Method = "POST";
                    request.ContentType = "application/x-www-form-urlencoded";
                    request.Timeout = 10000;
                    datastr = "username=" + userName + "&password=" + password;
                    using (dataStream = request.GetRequestStream())
                    {
                        string urlencoded = Uri.EscapeUriString(datastr);
                        byte[] data = encoding.GetBytes(urlencoded);
                        dataStream.Write(data, 0, data.Length);
                    }
                    WebResponse response = request.GetResponse();

                    using (dataStream = response.GetResponseStream())
                    {
                        StreamReader reader = new StreamReader(dataStream);
                        string responseFromServer = reader.ReadToEnd();
                        DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(LoginResponse));
                        using (Stream s = GenerateStreamFromString(responseFromServer))
                        {
                            LoginResponse loginresponse = (LoginResponse)serializer.ReadObject(s);
                            SessionId = loginresponse.SessionID;
                        }
                    }
                    HostName = serverURL;
                    UserName = userName;
                    Password = password;
                    logger.addEntryWithTimestamp("LOGIN \n\tuser@server: " + UserName + "@" + HostName + "\n\tsession id: " + SessionId);
                    return true;
                }
                catch (Exception)
                {
                    HostName = UserName = Password = SessionId = null;
                    return false;
                }
            }
            else
            {
                HostName = UserName = Password = SessionId = null;
                return false;
            }
        }

        public void Logout()
        {
            Console.WriteLine("Logout");
            try
            {
                Stream dataStream;
                WebRequest request;
                string datastr;
                UTF8Encoding encoding = new UTF8Encoding();

                request = WebRequest.Create( HostName + "?q=webservice/logout");
                request.Method = "POST";
                request.ContentType = "application/x-www-form-urlencoded";
                request.Timeout = 10000;
                datastr = "sessionid=" + SessionId;
                using (dataStream = request.GetRequestStream())
                {
                    string urlencoded = Uri.EscapeUriString(datastr);
                    byte[] data = encoding.GetBytes(urlencoded);
                    dataStream.Write(data, 0, data.Length);
                }
                WebResponse response = request.GetResponse();

                using (dataStream = response.GetResponseStream())
                {
                    StreamReader reader = new StreamReader(dataStream);
                    string responseFromServer = reader.ReadToEnd();
                }
            }
            catch (Exception e)
            {
                //Console.WriteLine(e.ToString());
                Messenger.Default.Send(new SendModalWindowMessage(e.ToString(), "ERROR"));
            }
            finally
            {
                logger.addEntryWithTimestamp("LOGOUT \n\tuser@server: " + UserName + "@" + HostName + "\n\tsession id: " + SessionId);
                resetConnection();
            }
        }



        public void resetConnection()
        {
            HostName = UserName = Password = SessionId = null;
        }




        #region INFORMATION_METHODS

        public bool isLoggedIn()
        {
            if (!String.IsNullOrEmpty(HostName) && !String.IsNullOrEmpty(UserName) && !String.IsNullOrEmpty(SessionId) && !String.IsNullOrEmpty(Password))
                return true;
            return false;
        }

        public bool IsNotLoggedIn()
        {
            return ! isLoggedIn();
        }

        public bool isConnected()
        {
            if ( ! isLoggedIn() )
            {
                //Messenger.Default.Send(new IsNotConnectedMessage("Please at first make a connection!"));
                throw new IsNotConnectedException();
            }
            return true;
        }

        #endregion




        #region GET_METHODS

        public GeneralAnswerData getCategories()
        {
            GeneralAnswerData generalAnswerData = new GeneralAnswerData();
            Console.WriteLine("getCategories");

            Stream dataStream;
            WebRequest request;

            string url = HostName + "?q=webservice/categories&sessionid=" + SessionId;
            request = WebRequest.Create(Uri.EscapeUriString(url));
            request.Method = "GET";

            //krequest.KeepAlive = true;
            request.Timeout = 10000;

            using (WebResponse response = request.GetResponse())
            {
                DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(CategoriesResponse));

                using (dataStream = response.GetResponseStream())
                {
                    CategoriesResponse resp = (CategoriesResponse)serializer.ReadObject(dataStream);
                    generalAnswerData.Categories = resp.Categories;
                    return generalAnswerData;
                }
            }
        }

        public GeneralAnswerData getItems()
        {
            GeneralAnswerData generalAnswerData = new GeneralAnswerData();
            Console.WriteLine("getItems");
            Stream dataStream;
            WebRequest request;

            string url = HostName + "?q=webservice/items&sessionid=" + SessionId;
            request = WebRequest.Create(Uri.EscapeUriString(url));
            request.Method = "GET";
            request.Timeout = 10000;
            using (WebResponse response = request.GetResponse())
            {
                DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(ItemsResponse));
                using (dataStream = response.GetResponseStream())
                {
                    ItemsResponse resp = (ItemsResponse)serializer.ReadObject(dataStream);
                    generalAnswerData.Items = resp.Items;
                    return generalAnswerData;
                }
            }
        }

        public GeneralAnswerData getPlans()
        {
            GeneralAnswerData generalAnswerData = new GeneralAnswerData();
            Console.WriteLine("getPlans");
            Stream dataStream;
            WebRequest request;

            string url = HostName + "?q=webservice/planitems&sessionid=" + SessionId;
            request = WebRequest.Create(Uri.EscapeUriString(url));
            request.Method = "GET";
            request.Timeout = 10000;
            using (WebResponse response = request.GetResponse())
            {
                DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(PlansResponse));
                using (dataStream = response.GetResponseStream())
                {
                    PlansResponse resp = (PlansResponse)serializer.ReadObject(dataStream);
                    generalAnswerData.Plans = resp.Plans;
                    return generalAnswerData;
                }
            }
        }

        public GeneralAnswerData getListsOfThings(bool getItems, bool getCategories, bool getPlans)
        {
            GeneralAnswerData generalAnswerData = new GeneralAnswerData();

            if (getItems)
            {
                GeneralAnswerData itemsAnswer = this.getItems();
                generalAnswerData.Items = itemsAnswer.Items;
            }
            if (getCategories)
            {
                GeneralAnswerData categoriesAnswer = this.getCategories();
                generalAnswerData.Categories = categoriesAnswer.Categories;
            }
            if (getPlans)
            {
                GeneralAnswerData plansAnswer = this.getPlans();
                generalAnswerData.Plans = plansAnswer.Plans;
            }

            if (generalAnswerData.Items == null && generalAnswerData.Categories == null && generalAnswerData.Plans == null)
                return null;

            return generalAnswerData;
        }



        #endregion




        #region ADD_METHODS

        public GeneralAnswerData createItem(GeneralParameterData toCreate)
        {
            Console.WriteLine("createItem");

            GeneralAnswerData generalAnswerData = new GeneralAnswerData(toCreate);
            Stream dataStream;
            WebRequest request;
            string datastr;
            UTF8Encoding encoding = new UTF8Encoding();

            string url = HostName + "?q=webservice/item_add&sessionid=" + SessionId;
            request = WebRequest.Create(Uri.EscapeUriString(url));
            request.Method = "POST";
            request.Timeout = 10000;
            request.ContentType = "application/x-www-form-urlencoded";
            datastr = "categoryid=" + toCreate.Category.Id + "&amount=" + toCreate.Item.Amount + "&description=" + toCreate.Item.Description;


            using (dataStream = request.GetRequestStream())
            {
                string urlencoded = Uri.EscapeUriString(datastr);
                byte[] data = encoding.GetBytes(urlencoded);
                dataStream.Write(data, 0, data.Length);
            }
            using (WebResponse response = request.GetResponse())
            {
                DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(ItemResponse));
                using (dataStream = response.GetResponseStream())
                {
                    ItemResponse resp = (ItemResponse)serializer.ReadObject(dataStream);
                }
            }

            return generalAnswerData;
        }

        public GeneralAnswerData createCategory(GeneralParameterData toCreate)
        {
            Console.WriteLine("createCategory");

            GeneralAnswerData generalAnswerData = new GeneralAnswerData(toCreate);
            Stream dataStream;
            WebRequest request;
            string datastr;
            UTF8Encoding encoding = new UTF8Encoding();

            string url = HostName + "?q=webservice/category_add&sessionid=" + SessionId;
            request = WebRequest.Create(Uri.EscapeUriString(url));
            request.Method = "POST";
            request.Timeout = 10000;
            request.ContentType = "application/x-www-form-urlencoded";
            datastr = "name=" + toCreate.Category.CategoryName + "&description=" + toCreate.Category.Description;


            using (dataStream = request.GetRequestStream())
            {
                string urlencoded = Uri.EscapeUriString(datastr);
                byte[] data = encoding.GetBytes(urlencoded);
                dataStream.Write(data, 0, data.Length);
            }
            using (WebResponse response = request.GetResponse())
            {
                //DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(CategoriesResponse));
                //using (dataStream = response.GetResponseStream())
                //{
                //    CategoriesResponse resp = (CategoriesResponse)serializer.ReadObject(dataStream);
                //}
            }

            return generalAnswerData;
        }

        public GeneralAnswerData createPlan(GeneralParameterData toCreate)
        {
            Console.WriteLine("createPlan");

            GeneralAnswerData generalAnswerData = new GeneralAnswerData(toCreate);
            Stream dataStream;
            WebRequest request;
            string datastr;
            UTF8Encoding encoding = new UTF8Encoding();

            string url = HostName + "?q=webservice/planitem_add&sessionid=" + SessionId;
            request = WebRequest.Create(Uri.EscapeUriString(url));
            request.Method = "POST";
            request.ContentType = "application/x-www-form-urlencoded";
            request.Timeout = 10000;
            datastr = "categoryid=" + toCreate.Category.Id + "&amount=" + toCreate.Plan.Amount + "&description=" + toCreate.Plan.Description;


            using (dataStream = request.GetRequestStream())
            {
                string urlencoded = Uri.EscapeUriString(datastr);
                byte[] data = encoding.GetBytes(urlencoded);
                dataStream.Write(data, 0, data.Length);
            }
            using (WebResponse response = request.GetResponse())
            {
                DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(PlansResponse));
                using (dataStream = response.GetResponseStream())
                {
                    PlansResponse resp = (PlansResponse)serializer.ReadObject(dataStream);
                }
            }
            return generalAnswerData;
        }

        #endregion




        #region EDIT_METHODS

        public GeneralAnswerData updateItem(GeneralParameterData toUpdate)
        {
            Console.WriteLine("updateItem");

            GeneralAnswerData generalAnswerData = new GeneralAnswerData(toUpdate);
            Stream dataStream;
            WebRequest request;
            string datastr;
            UTF8Encoding encoding = new UTF8Encoding();

            string url = HostName + "?q=webservice/item_edit&sessionid=" + SessionId;
            request = WebRequest.Create(Uri.EscapeUriString(url));
            request.Method = "POST";
            request.ContentType = "application/x-www-form-urlencoded";
            request.Timeout = 10000;
            datastr = "itemid=" + toUpdate.Item.Id + "&categoryid=" + toUpdate.Item.CategoryId
                + "&amount=" + toUpdate.Item.Amount + "&description=" + toUpdate.Item.Description;


            using (dataStream = request.GetRequestStream())
            {
                string urlencoded = Uri.EscapeUriString(datastr);
                byte[] data = encoding.GetBytes(urlencoded);
                dataStream.Write(data, 0, data.Length);
            }
            using (WebResponse response = request.GetResponse())
            {
                DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(ItemResponse));
                using (dataStream = response.GetResponseStream())
                {
                    ItemResponse resp = (ItemResponse)serializer.ReadObject(dataStream);
                }
            }
            return generalAnswerData;
        }

        public GeneralAnswerData updateCategory(GeneralParameterData toUpdate)
        {
            Console.WriteLine("updateCategory");

            GeneralAnswerData generalAnswerData = new GeneralAnswerData(toUpdate);
            Stream dataStream;
            WebRequest request;
            string datastr;
            UTF8Encoding encoding = new UTF8Encoding();

            string url = HostName + "?q=webservice/category_edit&sessionid=" + SessionId;
            request = WebRequest.Create(Uri.EscapeUriString(url));
            request.Method = "POST";
            request.ContentType = "application/x-www-form-urlencoded";
            request.Timeout = 10000;
            datastr = "categoryid=" + toUpdate.Category.Id + "&name=" + toUpdate.Category.CategoryName + "&description=" + toUpdate.Category.Description;


            using (dataStream = request.GetRequestStream())
            {
                string urlencoded = Uri.EscapeUriString(datastr);
                byte[] data = encoding.GetBytes(urlencoded);
                dataStream.Write(data, 0, data.Length);
            }
            using (WebResponse response = request.GetResponse())
            {
                DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(CategoryResponse));
                using (dataStream = response.GetResponseStream())
                {
                    CategoryResponse resp = (CategoryResponse)serializer.ReadObject(dataStream);

                    Console.WriteLine("Status : " + resp.Status);
                    Console.WriteLine("Success: " + resp.Success);
                    Console.WriteLine("Name   : " + resp.Category.CategoryName);
                    Console.WriteLine("Descr  : " + resp.Category.Description);
                    Console.WriteLine("Id     : " + resp.Category.Id);
                    Console.WriteLine("CreTime: " + resp.Category.CreationTime);
                    Console.WriteLine("ModTime: " + resp.Category.ModificationTime);                    
                }
            }
            return generalAnswerData;
        }

        public GeneralAnswerData updatePlan(GeneralParameterData toUpdate)
        {
            Console.WriteLine("updatePlan");

            GeneralAnswerData generalAnswerData = new GeneralAnswerData(toUpdate);
            Stream dataStream;
            WebRequest request;
            string datastr;
            UTF8Encoding encoding = new UTF8Encoding();

            string url = HostName + "?q=webservice/planitem_edit&sessionid=" + SessionId;
            request = WebRequest.Create(Uri.EscapeUriString(url));
            request.Method = "POST";
            request.ContentType = "application/x-www-form-urlencoded";
            request.Timeout = 10000;
            datastr = "planitemid=" + toUpdate.Plan.Id + "&amount=" + toUpdate.Plan.Amount + "&description="
                        + toUpdate.Plan.Description + "&categoryid=" + toUpdate.Plan.CategoryId;

            using (dataStream = request.GetRequestStream())
            {
                string urlencoded = Uri.EscapeUriString(datastr);
                byte[] data = encoding.GetBytes(urlencoded);
                dataStream.Write(data, 0, data.Length);
            }
            using (WebResponse response = request.GetResponse())
            {
                DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(PlanResponse));
                using (dataStream = response.GetResponseStream())
                {
                    PlanResponse resp = (PlanResponse)serializer.ReadObject(dataStream);
                }
            }
            return generalAnswerData;
        }

        #endregion




        #region DESTROY_METHODS

        public GeneralAnswerData destoryItem(GeneralParameterData toDestroy)
        {
            Console.WriteLine("destoryItem");
            GeneralAnswerData generalAnswerData = new GeneralAnswerData(toDestroy);


            Stream dataStream;
            WebRequest request;
            string datastr;
            UTF8Encoding encoding = new UTF8Encoding();

            string url = HostName + "?q=webservice/item_destroy&sessionid=" + SessionId;
            request = WebRequest.Create(Uri.EscapeUriString(url));
            request.Method = "POST";
            request.ContentType = "application/x-www-form-urlencoded";
            request.Timeout = 10000;
            datastr = "itemid=" + toDestroy.Item.Id;


            using (dataStream = request.GetRequestStream())
            {
                string urlencoded = Uri.EscapeUriString(datastr);
                byte[] data = encoding.GetBytes(urlencoded);
                dataStream.Write(data, 0, data.Length);
            }
            using (WebResponse response = request.GetResponse())
            {
                DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(NoDataResponse));
                using (dataStream = response.GetResponseStream())
                {
                    NoDataResponse resp = (NoDataResponse)serializer.ReadObject(dataStream);
                }
            }
            return generalAnswerData;
        }
                
        public GeneralAnswerData destoryCategory(GeneralParameterData toDestroy)
        {
            Console.WriteLine("destoryCategory");
            GeneralAnswerData generalAnswerData = new GeneralAnswerData(toDestroy);

            Stream dataStream;
            WebRequest request;
            string datastr;
            UTF8Encoding encoding = new UTF8Encoding();

            string url = HostName + "?q=webservice/category_destroy&sessionid=" + SessionId;
            request = WebRequest.Create(Uri.EscapeUriString(url));
            request.Method = "POST";
            request.ContentType = "application/x-www-form-urlencoded";
            request.Timeout = 10000;
            datastr = "categoryid=" + toDestroy.Category.Id;


            using (dataStream = request.GetRequestStream())
            {
                string urlencoded = Uri.EscapeUriString(datastr);
                byte[] data = encoding.GetBytes(urlencoded);
                dataStream.Write(data, 0, data.Length);
            }
            using (WebResponse response = request.GetResponse())
            {
                //DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(CategoriesResponse));
                //using (dataStream = response.GetResponseStream())
                //{
                //    CategoriesResponse resp = (CategoriesResponse)serializer.ReadObject(dataStream);
                //}
            }

            return generalAnswerData;
        }
        
        public GeneralAnswerData destoryPlan(GeneralParameterData toDestroy)
        {
            Console.WriteLine("destoryPlan");
            GeneralAnswerData generalAnswerData = new GeneralAnswerData(toDestroy);

            Stream dataStream;
            WebRequest request;
            string datastr;
            UTF8Encoding encoding = new UTF8Encoding();

            string url = HostName + "?q=webservice/planitem_destroy&sessionid=" + SessionId;
            request = WebRequest.Create(Uri.EscapeUriString(url));
            request.Method = "POST";
            request.ContentType = "application/x-www-form-urlencoded";
            request.Timeout = 10000;
            datastr = "planitemid=" + toDestroy.Plan.Id;


            using (dataStream = request.GetRequestStream())
            {
                string urlencoded = Uri.EscapeUriString(datastr);
                byte[] data = encoding.GetBytes(urlencoded);
                dataStream.Write(data, 0, data.Length);
            }
            using (WebResponse response = request.GetResponse())
            {
                //DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(CategoriesResponse));
                //using (dataStream = response.GetResponseStream())
                //{
                //    CategoriesResponse resp = (CategoriesResponse)serializer.ReadObject(dataStream);
                //}
            }
            return generalAnswerData;
        }

        #endregion


















        public GeneralAnswerData getPlansAndExpensesSummary(GeneralParameterData generalParameterData)
        {
            GeneralAnswerData generalAnswerData = new GeneralAnswerData(generalParameterData);
            Console.WriteLine("getPlansAndExpensesSummary");

            Stream dataStream;
            WebRequest request;

            string url = HostName + "?q=webservice/balance_stats&sessionid=" + SessionId;
            string datastr = "&categoryid=" + generalParameterData.SummaryOfPlansAndExpensesRequest.Categoryid
                    + "&granulation=" + generalParameterData.SummaryOfPlansAndExpensesRequest.Granulation
                    + "&year=" + generalParameterData.SummaryOfPlansAndExpensesRequest.Year;

            url += datastr;
            request = WebRequest.Create(Uri.EscapeUriString(url));
            request.Method = "GET";
            request.Timeout = 10000;

            using (WebResponse response = request.GetResponse())
            {
                DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(SummaryOfPlansAndExpensesResponse));

                using (dataStream = response.GetResponseStream())
                {
                    SummaryOfPlansAndExpensesResponse resp = (SummaryOfPlansAndExpensesResponse)serializer.ReadObject(dataStream);
                    generalAnswerData.SummaryOfPlansAndExpensesResults = resp.SummaryOfPlansAndExpensesResults;
                    return generalAnswerData;
                }
            }
        }


        public GeneralAnswerData getDailyCategoryExpensesOverview(GeneralParameterData generalParameterData)
        {
            GeneralAnswerData generalAnswerData = new GeneralAnswerData(generalParameterData);
            Console.WriteLine("getDailyCategoryExpensesOverview");

            Stream dataStream;
            WebRequest request;

            string url = HostName + "?q=webservice/daily_overview&sessionid=" + SessionId;
            string datastr = "&relative_month=" + generalParameterData.SummaryOfDailyCategoryExpensesRequest.RelativeMonth;

            url += datastr;
            request = WebRequest.Create(Uri.EscapeUriString(url));
            request.Method = "GET";
            request.Timeout = 10000;

            using (WebResponse response = request.GetResponse())
            {
                DataContractJsonSerializer serializer = new DataContractJsonSerializer(typeof(SummaryOfDailyCategoryExpensesResponse));

                using (dataStream = response.GetResponseStream())
                {
                    SummaryOfDailyCategoryExpensesResponse resp = (SummaryOfDailyCategoryExpensesResponse)serializer.ReadObject(dataStream);
                    generalAnswerData.SummaryOfDailyCategoryExpensesResults = resp.SummaryOfDailyCategoryExpensesResults;
                    return generalAnswerData;
                }
            }
        }





    }
}
