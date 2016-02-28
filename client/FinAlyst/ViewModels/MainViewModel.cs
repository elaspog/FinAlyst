using FinAlyst.Infrastructure;
using FinAlyst.Infrastructure.Exceptions;
using FinAlyst.Infrastructure.Helpers;
using FinAlyst.Infrastructure.MessageArgs;
using FinAlyst.Models;
using FinAlyst.Views;
using GalaSoft.MvvmLight;
using GalaSoft.MvvmLight.Command;
using GalaSoft.MvvmLight.Messaging;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Windows;


namespace FinAlyst.ViewModels
{
    /// <summary>
    /// This class contains properties that the main View can data bind to.
    /// <para>
    /// Use the <strong>mvvminpc</strong> snippet to add bindable properties to this ViewModel.
    /// </para>
    /// <para>
    /// You can also use Blend to data bind with the tool's support.
    /// </para>
    /// <para>
    /// See http://www.galasoft.ch/mvvm
    /// </para>
    /// </summary>
    public class MainViewModel : ViewModelBase
    {
        /// <summary>
        /// Initializes a new instance of the MainViewModel class.
        /// </summary>
        public MainViewModel()
        {
            logger = new Logger();
            Locator.LogsViewModel.setLogger(logger);
            repositoryManager = new RepositoryManager(logger);
            StatusMessage = "Please login to server.";
            
            repositoryManager.RaiseCategoriesQuerried += new RepositoryManager.Sign_GeneralAnswerData(onCategoriesQuerried);
            repositoryManager.RaisePlansQuerried += new RepositoryManager.Sign_GeneralAnswerData(onPlansQuerried);
            repositoryManager.RaiseItemsQuerried += new RepositoryManager.Sign_GeneralAnswerData(onItemsQuerried);
            repositoryManager.RaiseSummariesQuerried += new RepositoryManager.Sign_GeneralAnswerData(onSummariesQuerried);


            // TODO: ezeket is átírni Sign_GeneralAnswerData-re
            repositoryManager.RaiseLoginEnded += new RepositoryManager.Signature_Void_Bool(onLoginEnded);
            repositoryManager.RaiseLogoutEnded += new RepositoryManager.Signature_Void_Void(onLogoutEnded);
        }

        private ConfigurationDescriptor connectionsDescriptor;
        public ConfigurationDescriptor ConnectionsDescriptor
        {
            get { return connectionsDescriptor; }
            set
            {
                connectionsDescriptor = value;
                RaisePropertyChanged("ConnectionsDescriptor");
            }
        }
        
        
        SelectedSubViewToUpdateEnum selectedSubViewToUpdate;
        public SelectedSubViewToUpdateEnum SelectedSubViewToUpdate
        {
            get { return selectedSubViewToUpdate; }
            set 
            {
                selectedSubViewToUpdate = value;
                if (ServerConnection.isLoggedIn())
                {
                    loadData();
                }
            }
        }

        public IServerConnection ServerConnection
        {
            get { return RepositoryManager.SERVICES; }
        }

        /// Infrastructural variables
        ViewModelLocator    Locator             = new ViewModelLocator();
        public static Logger              logger;
        public RepositoryManager repositoryManager { get; private set; }

        private RelayCommand loginCommand;
        public RelayCommand LoginCommand
        {
            get { return loginCommand ?? (loginCommand = new RelayCommand(this.loginToServer, isNotLoggedInOrLogginIn)); }
        }        
        private RelayCommand logoutCommand;
        public RelayCommand LogoutCommand
        {
            get { return logoutCommand ?? (logoutCommand = new RelayCommand(this.logutFromServer, ServerConnection.isLoggedIn)); }
        }

        private RelayCommand aboutWindowCommand;
        public RelayCommand AboutWindowCommand
        {
            get { return aboutWindowCommand ?? (aboutWindowCommand = new RelayCommand(this.showAboutWindow)); }
        }

        private RelayCommand connectionSelectorWindowCommand;
        public RelayCommand ConnectionSelectorWindowCommand
        {
            get { return connectionSelectorWindowCommand ?? (connectionSelectorWindowCommand = new RelayCommand(this.showConnectionSelectorWindow)); }
        }



        private RelayCommand saveConnectionCommand;
        public RelayCommand SaveConnectionCommand
        {
            get { return saveConnectionCommand ?? (saveConnectionCommand = new RelayCommand(this.saveConnection)); }
        }

        private RelayCommand addConnectionCommand;
        public RelayCommand AddConnectionCommand
        {
            get { return addConnectionCommand ?? (addConnectionCommand = new RelayCommand(this.addConnection)); }
        }

        private RelayCommand removeConnectionCommand;
        public RelayCommand RemoveConnectionCommand
        {
            get { return removeConnectionCommand ?? (removeConnectionCommand = new RelayCommand(this.removeConnection)); }
        }

        private RelayCommand selectConnectionCommand;
        public RelayCommand SelectConnectionCommand
        {
            get { return selectConnectionCommand ?? (selectConnectionCommand = new RelayCommand(this.selectConnection)); }
        }






        
        private bool loginStatus = false;
        public bool LoginStatus
        {
            get { return loginStatus; }
            set
            {
                loginStatus = value;
                RaisePropertyChanged("LoginStatus");
                RaisePropertyChanged("NotLoginStatus");
            }
        }
        public bool NotLoginStatus
        {
            get { return ! loginStatus; }
        }



        private bool isNotLoggedInOrLogginIn()
        {
            return ! LoginStatus ;
        }


        private string statusMessage; 
        public string StatusMessage 
        {
            get { return statusMessage; }
            set 
            {
                statusMessage = value;
                RaisePropertyChanged("StatusMessage");
            } 
        }
        
        private Category[] categories;
        public Category[] Categories
        {
            get { return categories; }
            set
            {
                categories = value;
                RaisePropertyChanged("Categories");
            }
        }

        private Item[] items;
        public Item[] Items
        {
            get { return items; }
            set
            {
                items = value;
                RaisePropertyChanged("Items");
            }
        }

        private Plan[] plans;
        public Plan[] Plans
        {
            get { return plans; }
            set
            {
                plans = value;
                RaisePropertyChanged("Plans");
            }
        }

        string serverURL = null;

        string userName;
        public string UserName
        {
            get { return userName; }
            set
            {
                userName = value;
                RaisePropertyChanged("UserName");
            }
        }

        string password;
        public string Password
        {
            get { return password; }
            set
            {
                password = value;
                RaisePropertyChanged("Password");
            }
        }

        private void loginToServer()
        {
            LoginStatus = true;
            StatusMessage = "Connecting...";

            repositoryManager.Login(serverURL, UserName, Password);
        }

        private void logutFromServer()
        {
            LoginStatus = false;
            clearData();

            repositoryManager.Logout();
        }
        
        private void showAboutWindow()
        {
            Messenger.Default.Send(new OpenAboutWindowMessage());
        }


        string fileName = "config.xml";

        private void showConnectionSelectorWindow()
        {
            //repositoryManager.writeConfiguration("config_test_1.xml", new ConfigurationDescriptor(new ObservableCollection<string>(new List<string> { "sd","dfg"})));
            ConnectionsDescriptor = repositoryManager.loadConfiguration(fileName, new ConfigurationDescriptor
                (new ObservableCollection<string>(new List<string> { 
                    "http://example.localhost/fcgi-bin/finance", 
                    "http://www.example.somewebsite.com/fcgi-bin/finance"})));

            Messenger.Default.Send(new OpenConnectionSelectorWindowMessage());
        }

        private string selectConnectionInput;
        public string SelectConnectionInput
        {
            get { return selectConnectionInput; }
            set
            {
                selectConnectionInput = value;
                RaisePropertyChanged("SelectConnectionInput");
            }
        }
        //private string selectedConnection;
        //public string SelectedConnection
        //{
        //    get { return selectedConnection; }
        //    set
        //    {
        //        selectedConnection = value;
        //        RaisePropertyChanged("SelectedConnection");
        //    }
        //}
        private string selectedServerConnection;
        public string SelectedServerConnection
        {
            get { return selectedServerConnection; }
            set
            {
                selectedServerConnection = value;
                RaisePropertyChanged("SelectedServerConnection");
            }
        }


        private void saveConnection()
        {
            repositoryManager.writeConfiguration(fileName, ConnectionsDescriptor);

            Messenger.Default.Send(new SendModalWindowMessage("File config.xml saved.","Saved"));
        }

        private void addConnection()
        {
            if (SelectConnectionInput != null)
            {
                ConnectionsDescriptor.Connection.Add(SelectConnectionInput);
                SelectConnectionInput = null;
            }
            else
            {
                Messenger.Default.Send(new SendModalWindowMessage("Nothing to add.", "Error"));
            }
        }

        private void removeConnection()
        {
            if (SelectedServerConnection != null)
            {
                ConnectionsDescriptor.Connection.Remove(SelectedServerConnection);
            }
            else
            {
                Messenger.Default.Send(new SendModalWindowMessage("Item not selected.", "Error"));
            }
        }

        private void selectConnection()
        {
            if (SelectedServerConnection != null)
            {
                serverURL = SelectedServerConnection;

                Messenger.Default.Send(new CloseConnectionSelectorWindowMessage());
            }
        }


        public void loadData()
        {
            try
            {
                switch (SelectedSubViewToUpdate)
                {
                    case Infrastructure.SelectedSubViewToUpdateEnum.EXPENSES_VIEW:
                        repositoryManager.GetItems();
                        break;
                    case Infrastructure.SelectedSubViewToUpdateEnum.PLANS_VIEW:
                        repositoryManager.GetPlans();                      
                        break;
                    case Infrastructure.SelectedSubViewToUpdateEnum.CATEGORIES_VIEW:
                        repositoryManager.GetCategories();
                        break;
                    case Infrastructure.SelectedSubViewToUpdateEnum.SUMMARY_VIEW:
                        repositoryManager.GetSummaries();
                        break;
                }
            }
            catch (IsNotConnectedException)
            {
                Messenger.Default.Send(new SendModalWindowMessage("", ""));
                updateStatusBarAndLog("", "");
                clearData();
            }
            catch (Exception e)
            {
                //Console.WriteLine(e.ToString());
                Messenger.Default.Send(new SendModalWindowMessage( e.ToString(), "ERROR"));
            }
            finally 
            {
                //if (Categories == null || Items == null || Plans == null)
                //{
                //    Messenger.Default.Send(new SendModalWindowMessage("Querying data failed. Please try again.", "ERROR"));
                //    //ServerConnection.resetConnection();
                //    //clearData();
                //    logutFromServer();
                //}
            }
        }

        public void clearData()
        {
            Locator.CategoriesViewModel.clearTabContentViewModel();
            Locator.ExpensesViewModel.clearTabContentViewModel();
            Locator.PlansViewModel.clearTabContentViewModel();
            Locator.SummariesViewModel.clearTabContentViewModel();

            Categories  = null;
            Items       = null;
            Plans       = null;
        }

        public void reloadData()
        {
            clearData();
            loadData();
        }

        public void updateStatusBarAndLog(string statusBarMessage, string logMessage)
        {
            StatusMessage = statusBarMessage;
            logger.addEntryWithTimestamp(logMessage);
        }


        private void onLoginEnded(bool result)
        {
            if (result)
            {
                loadData();
                LoginStatus = true;
                StatusMessage = "Login succeeded.";
            }
            else
            {
                LoginStatus = false;
                StatusMessage = "Login failed.";
                Messenger.Default.Send(new SendModalWindowMessage("Server down or unreachable.", "Connection error."));
            }
        }

        private void onLogoutEnded()
        {
            StatusMessage = "Logged out.";
        }

        private void onCategoriesQuerried(GeneralAnswerData result)
        {
            if (result != null && result.Categories != null)
            {
                Categories = result.Categories;
                Application.Current.Dispatcher.Invoke((Action)(() => Locator.CategoriesViewModel.initTabContentViewModel()));
            }
        }

        private void onPlansQuerried(GeneralAnswerData result)
        {
            if (result != null && result.Plans != null)
            {
                Plans = result.Plans;
                Categories = result.Categories;
                Application.Current.Dispatcher.Invoke((Action)(() => Locator.PlansViewModel.initTabContentViewModel()));
            }
        }

        private void onItemsQuerried(GeneralAnswerData result)
        {
            if (result != null && result.Items != null && result.Categories != null)
            {
                Items = result.Items;
                Categories = result.Categories;
                Application.Current.Dispatcher.Invoke((Action)(() => Locator.ExpensesViewModel.initTabContentViewModel()));
            }
        }

        private void onSummariesQuerried(GeneralAnswerData result)
        {
            if (result != null && result.Categories != null)
            {
                Categories = result.Categories;
                Application.Current.Dispatcher.Invoke((Action)(() => Locator.SummariesViewModel.initTabContentViewModel()));
            }
        }

    }
}