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
    public class CategoriesViewModel : ViewModelBase, ITabContentViewModel
    {
        public CategoriesViewModel()
        {
            repositoryManager = Locator.MainViewModel.repositoryManager;
            repositoryManager.RaiseCategoryAdded += new RepositoryManager.Sign_GeneralAnswerData(onCategoryAdded);
            repositoryManager.RaiseCategorySaved += new RepositoryManager.Sign_GeneralAnswerData(onCategorySaved);
            repositoryManager.RaiseCategoryDeleted += new RepositoryManager.Sign_GeneralAnswerData(onCategoryDeleted);
        }



        #region VARIABLES

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

        private CategoryViewModel selectedCategory;
        public CategoryViewModel SelectedCategory
        {
            get { return selectedCategory; }
            set
            {
                selectedCategory = value;
                RaisePropertyChanged("SelectedCategory");
            }
        }

        private CategoryViewModel newCategory;
        public CategoryViewModel NewCategory
        {
            get { return newCategory; }
            set
            {
                newCategory = value;
                RaisePropertyChanged("NewCategory");
            }
        }

        #endregion

        #region COMMAND_DEFINITIONS

        private MyRelayCommand openExistingCategoryCommand;
        public ICommand OpenExistingCategoryCommand
        {
            get { return openExistingCategoryCommand ?? (openExistingCategoryCommand = new MyRelayCommand(param => this.openExistingCategory(param))); }
        }

        private RelayCommand addNewCategoryCommand;
        public RelayCommand AddNewCategoryCommand
        {
            get { return addNewCategoryCommand ?? (addNewCategoryCommand = new RelayCommand(this.addNewCategory)); }
        }
        
        private RelayCommand cancelNewCategoryCommand;
        public RelayCommand CancelNewCategoryCommand
        {
            get { return cancelNewCategoryCommand ?? (cancelNewCategoryCommand = new RelayCommand(this.cancelNewCategory)); }
        }
       
        private RelayCommand saveExistingCategoryCommand;
        public RelayCommand SaveExistingCategoryCommand
        {
            get { return saveExistingCategoryCommand ?? (saveExistingCategoryCommand = new RelayCommand(this.saveExistingCategory)); }
        }

        private RelayCommand deleteExistingCategoryCommand;
        public RelayCommand DeleteExistingCategoryCommand
        {
            get { return deleteExistingCategoryCommand ?? (deleteExistingCategoryCommand = new RelayCommand(this.deleteExistingCategory)); }
        }

        private RelayCommand clearExistingCategoryCommand;
        public RelayCommand ClearExistingCategoryCommand
        {
            get { return clearExistingCategoryCommand ?? (clearExistingCategoryCommand = new RelayCommand(this.clearExistingCategory)); }
        }

        #endregion

        #region COMMAND_METHODS

        private void openExistingCategory(object param)
        {
            SelectedCategory = (CategoryViewModel)param;
        }

        private void addNewCategory()
        {
            if (NewCategory != null)
            {
                GeneralParameterData generalParameterData = new GeneralParameterData();
                generalParameterData.Category = NewCategory.Category;

                repositoryManager.CreateCategory(generalParameterData);
            }
        }

        private void cancelNewCategory()
        {
            NewCategory = null;
            NewCategory = new CategoryViewModel(new Category());
        }

        private void saveExistingCategory()
        {
            if (SelectedCategory != null)
            {
                GeneralParameterData generalParameterData = new GeneralParameterData();
                generalParameterData.Category = SelectedCategory.Category;

                repositoryManager.UpdateCategory(generalParameterData);
            }
        }

        private void deleteExistingCategory()
        {
            try
            {
                if (SelectedCategory != null)
                {
                    GeneralParameterData generalParameterData = new GeneralParameterData();
                    generalParameterData.Category = SelectedCategory.Category;

                    repositoryManager.DestoryCategory(generalParameterData);
                }
            }
            catch (Exception)
            {
            }
        }

        private void clearExistingCategory()
        {
            SelectedCategory = null; 
        }

        #endregion


        #region EVENT_HANDLERS

        private void onCategoryAdded(GeneralAnswerData result)
        {
            cancelNewCategory();
            Locator.MainViewModel.reloadData();

            string msgLog = "NEW CATEGORY REQUEST: \n\tCategory Name: " + result.Category.CategoryName 
                        + " \n\tCategory Description: " + result.Category.Description + " \n\tCategory Id: " + result.Category.Id;
            string msgBar = "New category request: \t\tName: " + result.Category.CategoryName + " \tDescription: " + result.Category.Description;

            Locator.MainViewModel.updateStatusBarAndLog(msgBar, msgLog);
        }

        private void onCategorySaved(GeneralAnswerData result)
        {
            cancelNewCategory();
            Locator.MainViewModel.reloadData();

            string msgLog = "UPDATE CATEGORY REQUEST: \n\tCategory Name: " + result.Category.CategoryName
                        + " \n\tCategory Description: " + result.Category.Description + " \n\tCategory Id: " + result.Category.Id;
            string msgBar = "Update category request: \t\tName: " + result.Category.CategoryName + " \tDescription: " + result.Category.Description;

            Locator.MainViewModel.updateStatusBarAndLog(msgBar, msgLog);
        }


        private void onCategoryDeleted(GeneralAnswerData result)
        {
            clearExistingCategory();
            Locator.MainViewModel.reloadData();

            string msgLog = "DELETE CATEGORY REQUEST: \n\tCategory Name: " + result.Category.CategoryName
                + " \n\tCategory Description: " + result.Category.Description + " \n\tCategory Id: " + result.Category.Id;
            string msgBar = "Delete category request: \t\tName: " + result.Category.CategoryName + " \tDescription: " + result.Category.Description;
            Locator.MainViewModel.updateStatusBarAndLog(msgBar, msgLog);
        }

        #endregion 


        #region INTERFACE_METHODS ViewModelBase

        public void initTabContentViewModel()
        {
            NewCategory = new CategoryViewModel(new Category());

            ObservableCollection<Category> Categories = new ObservableCollection<Category>(Locator.MainViewModel.Categories);
            CategoryViewModels = new ObservableCollection<CategoryViewModel>();
            foreach (Category category in Categories)
            {
                CategoryViewModels.Add(new CategoryViewModel(category));
            }
        }

        public void clearTabContentViewModel()
        {
            CategoryViewModels = null;
            SelectedCategory = null;
            NewCategory = null;
        }

        #endregion

    }
}
