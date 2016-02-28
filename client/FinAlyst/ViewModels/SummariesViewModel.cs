using FinAlyst.Infrastructure;
using FinAlyst.Infrastructure.Helpers;
using FinAlyst.Models;
using FinAlyst.ViewModels.ViewModelEntities;
using FinAlyst.Views.Converters;
using GalaSoft.MvvmLight;
using GalaSoft.MvvmLight.Command;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FinAlyst.ViewModels
{
    public class SummariesViewModel : ViewModelBase, ITabContentViewModel
    {
        public SummariesViewModel()
        {
            repositoryManager = Locator.MainViewModel.repositoryManager;
            SelectedYear = DateTime.Now.Year.ToString();


            selectedMonth = DateTime.Now;

            repositoryManager.RaisePlansAndExpensesSummaryQuerried += new RepositoryManager.Sign_GeneralAnswerData(onPlansAndExpensesSummaryQuerried);
            repositoryManager.RaiseDailyCategoryExpensesSummaryQuerried += new RepositoryManager.Sign_GeneralAnswerData(onDailyCategoryExpensesSummaryQuerried);

            
        }

        ViewModelLocator Locator = new ViewModelLocator();
        RepositoryManager repositoryManager;




        private ObservableCollection<CategoryViewModel> categoryViewModels;
        public ObservableCollection<CategoryViewModel> CategoryViewModels
        {
            get { return categoryViewModels; }
            set
            {
                categoryViewModels = value;
                RaisePropertyChanged("CategoryViewModels");
            }
        }


        
        private CategoryViewModel plansAndExpensesSelectedCategoryViewModel;
        public CategoryViewModel PlansAndExpensesSelectedCategoryViewModel
        {
            get { return plansAndExpensesSelectedCategoryViewModel; }
            set
            {
                plansAndExpensesSelectedCategoryViewModel = value;
                RaisePropertyChanged("PlansAndExpensesSelectedCategoryViewModel");
            }
        }

        private string selectedYear;
        public string SelectedYear
        {
            get { return selectedYear; }
            set
            {
                selectedYear = value;
                RaisePropertyChanged("SelectedYear");
            }
        }

        private string checkValueInPlansAndExpenses;
        private bool isYearCheckedInPlansAndExpenses;
        private bool isMonthCheckedInPlansAndExpenses;
        private bool isWeekCheckedInPlansAndExpenses;

        public bool IsYearCheckedInPlansAndExpenses
        {
            get { return isYearCheckedInPlansAndExpenses; }
            set
            {
                isYearCheckedInPlansAndExpenses = value;
                RaisePropertyChanged("IsYearCheckedInPlansAndExpenses");
                if (isYearCheckedInPlansAndExpenses)
                    checkValueInPlansAndExpenses = "year";
            }
        }
        public bool IsMonthCheckedInPlansAndExpenses
        {
            get { return isMonthCheckedInPlansAndExpenses; }
            set
            {
                isMonthCheckedInPlansAndExpenses = value;
                RaisePropertyChanged("IsMonthCheckedInPlansAndExpenses");
                if (isMonthCheckedInPlansAndExpenses)
                    checkValueInPlansAndExpenses = "month";
            }
        }
        public bool IsWeekCheckedInPlansAndExpenses
        {
            get { return isWeekCheckedInPlansAndExpenses; }
            set
            {
                isWeekCheckedInPlansAndExpenses = value;
                RaisePropertyChanged("IsWeekCheckedInPlansAndExpenses");
                if (isWeekCheckedInPlansAndExpenses)
                    checkValueInPlansAndExpenses = "week";
            }
        }
        private Collection<SummaryOfPlansAndExpensesResult> summariesOfPlansAndExpensesResult;
        public Collection<SummaryOfPlansAndExpensesResult> SummariesOfPlansAndExpensesResult
        {
            get { return summariesOfPlansAndExpensesResult; }
            set
            {
                summariesOfPlansAndExpensesResult = value;
                RaisePropertyChanged("SummariesOfPlansAndExpensesResult");
            }
        }

        private string summariesOfPlansAndExpensesTitle;
        public string SummariesOfPlansAndExpensesTitle
        {
            get { return summariesOfPlansAndExpensesTitle; }
            set
            {
                summariesOfPlansAndExpensesTitle = value;
                RaisePropertyChanged("SummariesOfPlansAndExpensesTitle");
            }
        }



        private DateTime selectedMonth;
        public DateTime SelectedMonth
        {
            get { return selectedMonth; }
            set
            {
                DateTime rightNow = DateTime.Now;

                if (value <= rightNow)
                {
                    selectedMonth = value;
                    RaisePropertyChanged("SelectedMonth");
                    RaisePropertyChanged("SelectedMonthStr");
                    RaisePropertyChanged("RelativeSelectedMonth");
                }
            }
        }
        public string SelectedMonthStr
        {
            get { return selectedMonth.Year + "-" + selectedMonth.Month; }
        }
        public int RelativeSelectedMonth
        {
            get 
            {
                DateTime rightNow = DateTime.Now;
                return (rightNow.Month - selectedMonth.Month) + 12 * (rightNow.Year - selectedMonth.Year);
            }
        }


        private ColumnConfig columnConfig;
        public ColumnConfig ColumnConfig
        {
            get { return columnConfig; }
            set
            {
                columnConfig = value;
                RaisePropertyChanged("ColumnConfig");
            }
        }


        private Collection<SummaryOfDailyCategoryExpensesResult> summariesOfDailyCategoryExpensesResult;
        public Collection<SummaryOfDailyCategoryExpensesResult> SummariesOfDailyCategoryExpensesResult
        {
            get { return summariesOfDailyCategoryExpensesResult; }
            set
            {
                summariesOfDailyCategoryExpensesResult = value;
                RaisePropertyChanged("SummariesOfDailyCategoryExpensesResult");
            }
        }

        private string summariesOfDailyCategoryExpensesTitle;
        public string SummariesOfDailyCategoryExpensesTitle
        {
            get { return summariesOfDailyCategoryExpensesTitle; }
            set
            {
                summariesOfDailyCategoryExpensesTitle = value;
                RaisePropertyChanged("SummariesOfDailyCategoryExpensesTitle");
            }
        }




        private RelayCommand prevYearCommand;
        public RelayCommand PrevYearCommand
        {
            get { return prevYearCommand ?? (prevYearCommand = new RelayCommand(this.setPreyYear)); }
        }

        private RelayCommand nextYearCommand;
        public RelayCommand NextYearCommand
        {
            get { return nextYearCommand ?? (nextYearCommand = new RelayCommand(this.setNextYear)); }
        }

        private RelayCommand clearPlansAndExpensesSummaryCommand;
        public RelayCommand ClearPlansAndExpensesSummaryCommand
        {
            get { return clearPlansAndExpensesSummaryCommand ?? (clearPlansAndExpensesSummaryCommand = new RelayCommand(this.clearPlansAndExpensesSummary)); }
        }

        private RelayCommand generatePlansAndExpensesSummaryCommand;
        public RelayCommand GeneratePlansAndExpensesSummaryCommand
        {
            get { return generatePlansAndExpensesSummaryCommand ?? (generatePlansAndExpensesSummaryCommand = new RelayCommand(this.generatePlansAndExpensesSummary)); }
        }
        
        private RelayCommand prevMonthCommand;
        public RelayCommand PrevMonthCommand
        {
            get { return prevMonthCommand ?? (prevMonthCommand = new RelayCommand(this.setPrevMonth)); }
        }
        private RelayCommand nextMonthCommand;
        public RelayCommand NextMonthCommand
        {
            get { return nextMonthCommand ?? (nextMonthCommand = new RelayCommand(this.setNextMonth)); }
        }

        private RelayCommand clearDailyCategoryExpensesSummaryCommand;
        public RelayCommand ClearDailyCategoryExpensesSummaryCommand
        {
            get { return clearDailyCategoryExpensesSummaryCommand ?? (clearDailyCategoryExpensesSummaryCommand = new RelayCommand(this.clearDailyCategoryExpensesSummary)); }
        }

        private RelayCommand generateDailyCategoryExpensesSummaryCommand;
        public RelayCommand GenerateDailyCategoryExpensesSummaryCommand
        {
            get { return generateDailyCategoryExpensesSummaryCommand ?? (generateDailyCategoryExpensesSummaryCommand = new RelayCommand(this.generateDailyCategoryExpensesSummary)); }
        }

        
        
        private void setPreyYear()
        {
            int year = int.Parse(SelectedYear);
            year--;
            SelectedYear = year.ToString();
        }
        private void setNextYear()
        {
            int thisYear = DateTime.Now.Year;
            int actualYearSet = int.Parse(SelectedYear);

            if (actualYearSet < thisYear)
            {
                thisYear++;
                actualYearSet++;
                SelectedYear = actualYearSet.ToString();
            }
        }
        private void clearPlansAndExpensesSummary()
        {
            SelectedYear = DateTime.Now.Year.ToString();
            PlansAndExpensesSelectedCategoryViewModel = null;
            checkValueInPlansAndExpenses = null;
            IsYearCheckedInPlansAndExpenses = false;
            IsMonthCheckedInPlansAndExpenses = false;
            IsWeekCheckedInPlansAndExpenses = false;
            SummariesOfPlansAndExpensesTitle = null;
            SummariesOfPlansAndExpensesResult = null;
        }
        private void generatePlansAndExpensesSummary()
        {
            if (SelectedYear != null && checkValueInPlansAndExpenses != null && PlansAndExpensesSelectedCategoryViewModel != null)
            {
                GeneralParameterData generalParameterData = new GeneralParameterData();

                generalParameterData.SummaryOfPlansAndExpensesRequest = new SummaryOfPlansAndExpensesRequest();

                generalParameterData.SummaryOfPlansAndExpensesRequest.Granulation = checkValueInPlansAndExpenses;
                generalParameterData.SummaryOfPlansAndExpensesRequest.Categoryid = PlansAndExpensesSelectedCategoryViewModel.Id.ToString();
                generalParameterData.SummaryOfPlansAndExpensesRequest.Year = SelectedYear;

                repositoryManager.GetPlansAndExpensesSummary(generalParameterData);
            }
        }


        private void setPrevMonth()
        {
            SelectedMonth = SelectedMonth.AddMonths(-1);
            RaisePropertyChanged("SelectedMonth");
            RaisePropertyChanged("SelectedMonthStr");
            RaisePropertyChanged("RelativeSelectedMonth");
        }

        private void setNextMonth()
        {
            SelectedMonth = SelectedMonth.AddMonths(1);
            RaisePropertyChanged("SelectedMonth");
            RaisePropertyChanged("SelectedMonthStr");
            RaisePropertyChanged("RelativeSelectedMonth");
        }

        private void clearDailyCategoryExpensesSummary()
        {
            SelectedMonth = DateTime.Now;
            SummariesOfDailyCategoryExpensesTitle = null;
            SummariesOfDailyCategoryExpensesResult = null;
        }

        private void generateDailyCategoryExpensesSummary()
        {
            if (SelectedMonth != null)
            {
                GeneralParameterData generalParameterData = new GeneralParameterData();

                generalParameterData.SummaryOfDailyCategoryExpensesRequest = new SummaryOfDailyCategoryExpensesRequest();

                generalParameterData.SummaryOfDailyCategoryExpensesRequest.RelativeMonth = RelativeSelectedMonth.ToString();
                generalParameterData.SummaryOfDailyCategoryExpensesRequest.SelectedMonthStr = SelectedMonthStr;

                repositoryManager.GetDailyCategoryExpensesSummary(generalParameterData);
            }
        }




        private void onPlansAndExpensesSummaryQuerried(GeneralAnswerData generalAnswerData)
        {
            SummariesOfPlansAndExpensesResult = new Collection<SummaryOfPlansAndExpensesResult>(generalAnswerData.SummaryOfPlansAndExpensesResults);

            SummariesOfPlansAndExpensesTitle = "Income and Expenses \n(on category "  
                    + CategoryViewModels.Where(x => x.Id == PlansAndExpensesSelectedCategoryViewModel.Id).FirstOrDefault().CategoryName 
                    + " by granulation " + generalAnswerData.SummaryOfPlansAndExpensesRequest.Granulation 
                    + " in year " + generalAnswerData.SummaryOfPlansAndExpensesRequest.Year 
                    + ")";
        }

        private void onDailyCategoryExpensesSummaryQuerried(GeneralAnswerData generalAnswerData)
        {
            ColumnConfig = new ColumnConfig();
            //Category actualCategory = null;

            //foreach (SummaryOfDailyCategoryExpensesResult summaryOfDailyCategoryExpensesResult in generalAnswerData.SummaryOfDailyCategoryExpensesResults)
            //{
            //    if (actualCategory == null || actualCategory.Id != summaryOfDailyCategoryExpensesResult.Category.Id)
            //    {
            //        actualCategory = summaryOfDailyCategoryExpensesResult.Category;
            //        ColumnConfig.Columns.Add(new Column { Header = actualCategory.CategoryName, DataField = "Name" });
            //    }
            
            //}
            

            SummariesOfDailyCategoryExpensesResult = new Collection<SummaryOfDailyCategoryExpensesResult>(generalAnswerData.SummaryOfDailyCategoryExpensesResults);




            SummariesOfPlansAndExpensesTitle = "Daily expenses by category \n(on category "
                    + " in month " + generalAnswerData.SummaryOfDailyCategoryExpensesRequest.SelectedMonthStr
                    + ")";
        }


        public void initTabContentViewModel()
        {
            ObservableCollection<Category> Categories = new ObservableCollection<Category>(Locator.MainViewModel.Categories);

            CategoryViewModels = new ObservableCollection<CategoryViewModel>();

            foreach (Category category in Categories)
            {
                CategoryViewModels.Add(new CategoryViewModel(category));
            }
            SelectedYear = DateTime.Now.Year.ToString();
        }

        public void clearTabContentViewModel()
        {
            CategoryViewModels = null;
            SelectedYear = null;
            SummariesOfPlansAndExpensesTitle = null;
            PlansAndExpensesSelectedCategoryViewModel = null;

        }
    }
}
