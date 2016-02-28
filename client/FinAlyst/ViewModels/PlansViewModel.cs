using FinAlyst.Infrastructure;
using FinAlyst.Infrastructure.Helpers;
using FinAlyst.Models;
using FinAlyst.ViewModels.ViewModelEntities;
using GalaSoft.MvvmLight;
using GalaSoft.MvvmLight.Command;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Input;

namespace FinAlyst.ViewModels
{
    public class PlansViewModel : ViewModelBase, ITabContentViewModel
    {
        public PlansViewModel()
        {
            repositoryManager = Locator.MainViewModel.repositoryManager;
            repositoryManager.RaisePlanAdded += new RepositoryManager.Sign_GeneralAnswerData(onPlanAdded);
            repositoryManager.RaisePlanDeleted += new RepositoryManager.Sign_GeneralAnswerData(onPlanDeleted);
            repositoryManager.RaisePlanSaved += new RepositoryManager.Sign_GeneralAnswerData(onPlanSaved);
        }
        
        private ObservableCollection<PlanViewModel> planViewModels;
        public ObservableCollection<PlanViewModel> PlanViewModels
        {
            get { return planViewModels; }
            set
            {
                planViewModels = value;
                RaisePropertyChanged("PlanViewModels");
            }
        }

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

        private CategoryViewModel adderSelectedCategoryViewModel;
        public CategoryViewModel AdderSelectedCategoryViewModel
        {
            get { return adderSelectedCategoryViewModel; }
            set
            {
                adderSelectedCategoryViewModel = value;
                RaisePropertyChanged("AdderSelectedCategoryViewModel");
            }
        }

        private CategoryViewModel editorSelectedCategoryViewModel;
        public CategoryViewModel EditorSelectedCategoryViewModel
        {
            get { return editorSelectedCategoryViewModel; }
            set
            {
                editorSelectedCategoryViewModel = value;
                RaisePropertyChanged("EditorSelectedCategoryViewModel");
            }
        }

        ViewModelLocator Locator = new ViewModelLocator();

        RepositoryManager repositoryManager;
        
        public PlanViewModel selectedPlan;
        public PlanViewModel SelectedPlan 
        {
            get { return selectedPlan; }
            set
            {
                selectedPlan = value;
                RaisePropertyChanged("SelectedPlan");
            }
        }

        private PlanViewModel newPlan;
        public PlanViewModel NewPlan
        {
            get { return newPlan; }
            set
            {
                newPlan = value;
                RaisePropertyChanged("NewPlan");
            }
        }


        private MyRelayCommand openExistingPlanCommand;
        public ICommand OpenExistingPlanCommand
        {
            get { return openExistingPlanCommand ?? (openExistingPlanCommand = new MyRelayCommand(param => this.openExistingPlan(param))); }
        }

        private RelayCommand addNewPlanCommand;
        public RelayCommand AddNewPlanCommand
        {
            get { return addNewPlanCommand ?? (addNewPlanCommand = new RelayCommand(this.addNewPlan)); }
        }

        private RelayCommand cancelNewPlanCommand;
        public RelayCommand CancelNewPlanCommand
        {
            get { return cancelNewPlanCommand ?? (cancelNewPlanCommand = new RelayCommand(this.cancelNewPlan)); }
        }

        private RelayCommand saveExistingPlanCommand;
        public RelayCommand SaveExistingPlanCommand
        {
            get { return saveExistingPlanCommand ?? (saveExistingPlanCommand = new RelayCommand(this.saveExistingPlan)); }
        }

        private RelayCommand deleteExistingPlanCommand;
        public RelayCommand DeleteExistingPlanCommand
        {
            get { return deleteExistingPlanCommand ?? (deleteExistingPlanCommand = new RelayCommand(this.deleteExistingPlan)); }
        }

        private RelayCommand clearExistingPlanCommand;
        public RelayCommand ClearExistingPlanCommand
        {
            get { return clearExistingPlanCommand ?? (clearExistingPlanCommand = new RelayCommand(this.clearExistingPlan)); }
        }


        private void openExistingPlan(object param)
        {
            SelectedPlan = (PlanViewModel)param;
            CategoryViewModel SelectedItemCategoryViewModel = CategoryViewModels.Where(x => x.Id == SelectedPlan.CategoryId).FirstOrDefault();
            EditorSelectedCategoryViewModel = SelectedItemCategoryViewModel;
        }

        private void addNewPlan()
        {
            if (NewPlan != null)
            {
                GeneralParameterData generalParameterData = new GeneralParameterData();

                generalParameterData.Category = AdderSelectedCategoryViewModel.Category;
                generalParameterData.Plan = NewPlan.Plan;

                repositoryManager.CreatePlan(generalParameterData);
            }
        }

        private void cancelNewPlan()
        {
            NewPlan = null;
            NewPlan = new PlanViewModel(new Plan());
        }
        
        private void saveExistingPlan()
        {
            if (SelectedPlan != null)
            {
                GeneralParameterData generalParameterData = new GeneralParameterData();
                generalParameterData.Plan = SelectedPlan.Plan;
                generalParameterData.Plan.CategoryId = EditorSelectedCategoryViewModel.Id;

                repositoryManager.UpdatePlan(generalParameterData);
            }
        }

        private void deleteExistingPlan()
        {
            try
            {
                if (SelectedPlan != null)
                {
                    GeneralParameterData generalParameterData = new GeneralParameterData();
                    generalParameterData.Plan = SelectedPlan.Plan;

                    repositoryManager.DeletePlan(generalParameterData);
                }
            }
            catch (Exception)
            {
            }
        }

        private void clearExistingPlan()
        {
            SelectedPlan = null; 
        }



        private void openExistingPlan()
        {
            throw new System.NotImplementedException();
        }






        private void onPlanAdded(GeneralAnswerData result)
        {
            cancelNewPlan();
            Locator.MainViewModel.reloadData();

            string msgLog = "NEW PLAN REQUEST: \n\tPlan Description: " + result.Plan.Description
                    + " \n\tPlan Amount: " + result.Plan.Amount + " \n\tPlan's category Id: " + result.Plan.CategoryId;
            string msgBar = "New plan request: \t\tDescription: " + result.Plan.Description + " \tAmount: " + result.Plan.Amount;

            Locator.MainViewModel.updateStatusBarAndLog(msgBar, msgLog);
        }

        private void onPlanDeleted(GeneralAnswerData result)
        {
            clearExistingPlan();
            Locator.MainViewModel.reloadData();

            string msgLog = "DELETE PLAN REQUEST: \n\tDescription: " + result.Plan.Description
                + " \n\tAmount: " + result.Plan.Amount + " \n\tPlan Id: " + result.Plan.Id ;
            string msgBar = "Delete plan request: \t\tDescription: " + result.Plan.Description + " \tAmount: " + result.Plan.Amount;
            Locator.MainViewModel.updateStatusBarAndLog(msgBar, msgLog);
        }

        private void onPlanSaved(GeneralAnswerData result)
        {
            clearExistingPlan();
            Locator.MainViewModel.reloadData();

            string msgLog = "UPDATE PLAN REQUEST: \n\tDescription: " + result.Plan.Description
                + " \n\tAmount: " + result.Plan.Amount + " \n\tPlan Id: " + result.Plan.Id;
            string msgBar = "Update plan request: \t\tDescription: " + result.Plan.Description + " \tAmount: " + result.Plan.Amount;
            Locator.MainViewModel.updateStatusBarAndLog(msgBar, msgLog);
        }







        public void initTabContentViewModel()
        {
            NewPlan = new PlanViewModel(new Plan());
            SelectedPlan = null;

            ObservableCollection<Plan> Plans = new ObservableCollection<Plan>(Locator.MainViewModel.Plans);
            ObservableCollection<Category> Categories = new ObservableCollection<Category>(Locator.MainViewModel.Categories); 

            PlanViewModels = new ObservableCollection<PlanViewModel>();
            CategoryViewModels = new ObservableCollection<CategoryViewModel>();

            foreach (Plan plan in Plans)
            {
                PlanViewModels.Add(new PlanViewModel(plan));
            }
            foreach (Category category in Categories)
            {
                CategoryViewModels.Add(new CategoryViewModel(category));
            }
        }

        public void clearTabContentViewModel()
        {
            PlanViewModels = null;
            NewPlan = null;
            SelectedPlan = null;
            EditorSelectedCategoryViewModel = null;
            AdderSelectedCategoryViewModel = null;
        }


    }
}
