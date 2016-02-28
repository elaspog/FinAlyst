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
    public class ExpensesViewModel : ViewModelBase, ITabContentViewModel
    {
        public ExpensesViewModel()
        {
            repositoryManager = Locator.MainViewModel.repositoryManager;
            repositoryManager.RaiseItemAdded += new RepositoryManager.Sign_GeneralAnswerData(onItemAdded);
            repositoryManager.RaiseItemDeleted += new RepositoryManager.Sign_GeneralAnswerData(onItemDeleted);
            repositoryManager.RaiseItemSaved += new RepositoryManager.Sign_GeneralAnswerData(onItemSaved);
        }

        RepositoryManager repositoryManager;

        private ObservableCollection<ItemViewModel> itemViewModels;
        public ObservableCollection<ItemViewModel> ItemViewModels
        {
            get { return itemViewModels; }
            set
            {
                itemViewModels = value;
                RaisePropertyChanged("ItemViewModels");
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

        private ItemViewModel selectedItem;
        public ItemViewModel SelectedItem
        {
            get { return selectedItem; }
            set
            {
                selectedItem = value;
                RaisePropertyChanged("SelectedItem");
            }
        }

        private ItemViewModel newItem;
        public ItemViewModel NewItem
        {
            get { return newItem; }
            set
            {
                newItem = value;
                RaisePropertyChanged("NewItem");
            }
        }



        private MyRelayCommand openExistingItemCommand;
        public ICommand OpenExistingItemCommand
        {
            get { return openExistingItemCommand ?? (openExistingItemCommand = new MyRelayCommand(param => this.openExistingItem(param))); }
        }

        private RelayCommand addNewItemCommand;
        public RelayCommand AddNewItemCommand
        {
            get { return addNewItemCommand ?? (addNewItemCommand = new RelayCommand(this.addNewItem)); }
        }


        private RelayCommand cancelNewItemCommand;
        public RelayCommand CancelNewItemCommand
        {
            get { return cancelNewItemCommand ?? (cancelNewItemCommand = new RelayCommand(this.cancelNewItem)); }
        }


        private RelayCommand saveExistingItemCommand;
        public RelayCommand SaveExistingItemCommand
        {
            get { return saveExistingItemCommand ?? (saveExistingItemCommand = new RelayCommand(this.saveExistingItem)); }
        }

        private RelayCommand deleteExistingItemCommand;
        public RelayCommand DeleteExistingItemCommand
        {
            get { return deleteExistingItemCommand ?? (deleteExistingItemCommand = new RelayCommand(this.deleteExistingItem)); }
        }

        private RelayCommand clearExistingItemCommand;
        public RelayCommand ClearExistingItemCommand
        {
            get { return clearExistingItemCommand ?? (clearExistingItemCommand = new RelayCommand(this.clearExistingItem)); }
        }


        
        private void openExistingItem(object param)
        {
            SelectedItem = (ItemViewModel)param;
            CategoryViewModel SelectedItemCategoryViewModel = CategoryViewModels.Where(x => x.Id == SelectedItem.CategoryId).FirstOrDefault();
            EditorSelectedCategoryViewModel = SelectedItemCategoryViewModel;
        }

        private void addNewItem()
        {
            if (NewItem != null)
            {
                GeneralParameterData generalParameterData = new GeneralParameterData();

                generalParameterData.Category = AdderSelectedCategoryViewModel.Category;
                generalParameterData.Item = NewItem.Item;


                string newItemCategoryId = AdderSelectedCategoryViewModel.Category.Id.ToString();
                string newItemDescription = NewItem.Description;
                string newItemAmount = NewItem.Amount.ToString();

                repositoryManager.CreateItem(generalParameterData);


                //string newItemCategoryId = AdderSelectedCategoryViewModel.Category.Id.ToString();
                //string newItemDescription = NewItem.Description;
                //string newItemAmount = NewItem.Amount.ToString();

                //ServerManager.addAndRunRequestAsPossible<Func<GeneralParameterData, GeneralAnswerData>>
                //    ((a, b, c) => ServerManager.SERVICES.createItem(newItemCategoryId, newItemDescription, newItemAmount), "AddItem");
            }
        }
        private void cancelNewItem()
        {
            NewItem = null;
            AdderSelectedCategoryViewModel = null;
            //NewItem = new ItemViewModel(new Item());
        }
        private void saveExistingItem()
        {
            if (SelectedItem != null)
            {
                GeneralParameterData generalParameterData = new GeneralParameterData();
                generalParameterData.Item = SelectedItem.Item;
                generalParameterData.Item.CategoryId = EditorSelectedCategoryViewModel.Id;

                repositoryManager.UpdateItem(generalParameterData);
            }
        }

        private void deleteExistingItem()
        {
            try
            {
                if (SelectedItem != null)
                {
                    GeneralParameterData generalParameterData = new GeneralParameterData();
                    generalParameterData.Item = SelectedItem.Item;

                    repositoryManager.DestoryItem(generalParameterData);
                }
            }
            catch (Exception)
            {
            }
        }

        private void clearExistingItem()
        {
            SelectedItem = null; 
            EditorSelectedCategoryViewModel = null;
        }


        

        private void onItemAdded(GeneralAnswerData result)
        {
            cancelNewItem();
            Locator.MainViewModel.reloadData();

            string msgLog = "NEW ITEM REQUEST: \n\tItem Name: " + result.Item.Description 
                + " \n\tItem Description: " + result.Item.Amount + " \n\tItem's category Id: " + result.Item.CategoryId;
            string msgBar = "New item request: \t\tName: " + result.Item.Description + " \tAmount: " + result.Item.Amount;

            Locator.MainViewModel.updateStatusBarAndLog(msgBar, msgLog);
        }

        private void onItemDeleted(GeneralAnswerData result)
        {
            clearExistingItem();
            Locator.MainViewModel.reloadData();

            string msgLog = "DELETE ITEM REQUEST: \n\tItem Description: " + result.Item.Description + " \n\tItem Amount: " + result.Item.Amount + " \n\tItem Id: " + result.Item.Id;
            string msgBar = "Delete item request: \t\tDescription: " + result.Item.Description + " \tAmount: " + result.Item.Amount;
            Locator.MainViewModel.updateStatusBarAndLog(msgBar, msgLog);
        }

        private void onItemSaved(GeneralAnswerData result)
        {
            clearExistingItem();
            Locator.MainViewModel.reloadData();

            string msgLog = "SAVE ITEM REQUEST: \n\tItem Description: " + result.Item.Description + " \n\tItem Amount: " + result.Item.Amount + " \n\tItem Id: " + result.Item.Id;
            string msgBar = "Save item request: \t\tDescription: " + result.Item.Description + " \tAmount: " + result.Item.Amount;
            Locator.MainViewModel.updateStatusBarAndLog(msgBar, msgLog);
        }

        


        ViewModelLocator Locator = new ViewModelLocator();

        public void initTabContentViewModel()
        {
            NewItem = new ItemViewModel(new Item());
            SelectedItem = null;

            ObservableCollection<Item> Items = new ObservableCollection<Item>(Locator.MainViewModel.Items);
            ObservableCollection<Category> Categories = new ObservableCollection<Category>(Locator.MainViewModel.Categories); 

            ItemViewModels = new ObservableCollection<ItemViewModel>();
            CategoryViewModels = new ObservableCollection<CategoryViewModel>();

            foreach (Item item in Items)
            {
                ItemViewModels.Add(new ItemViewModel(item));
            }
            foreach (Category category in Categories)
            {
                CategoryViewModels.Add(new CategoryViewModel(category));
            }
        }

        public void clearTabContentViewModel()
        {
            ItemViewModels = null;
            SelectedItem = null;
            NewItem = null;
            EditorSelectedCategoryViewModel = null;
            AdderSelectedCategoryViewModel = null;
        }
    }
}
