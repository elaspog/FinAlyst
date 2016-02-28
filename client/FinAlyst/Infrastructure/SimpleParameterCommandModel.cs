using GalaSoft.MvvmLight.Command;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FinAlyst.ViewModels.Helpers
{

    // http://stackoverflow.com/questions/3476792/wpf-command-parameter-for-a-delete-command-in-a-list

    //public sealed class SimpleParameterCommandModel<T> : CommandModel
    //{
    //    private readonly Action<T> execute;
    //    private readonly Func<T, bool> canExecute;

    //    public SimpleParameterCommandModel(string label, string tooltip, Action<T> execute, Func<T, bool> canExecute)
    //        : base(appCtx, dataCtx, label, tooltip)
    //    {
    //        if (execute == null) throw new ArgumentNullException("execute");
    //        this.execute = execute;
    //        this.canExecute = canExecute;
    //    }
    //    ...
    //}





    //public sealed class SimpleParameterCommandModel : RelayCommand
    //{
    //    private readonly Action execute;
    //    private readonly Func<bool> canExecute;

    //    public SimpleParameterCommandModel(Action execute, Func<bool> canExecute)
    //        : base(execute, canExecute)
    //    {
    //        if (execute == null) throw new ArgumentNullException("execute");
    //        this.execute = execute;
    //        this.canExecute = canExecute;
    //    }

    //}



    //USAGE:

    //private ICommand _DeleteCommand = null;
    //public ICommand DeleteCommand
    //{
    //    get
    //    {
    //        if (_DeleteCommand == null)
    //        {
    //            _DeleteCommand = new SimpleParameterCommandModel<IEnumerable<DataObjectModel>>                      ("Delete", "Delete selection from data store", 
    //                (items) => items.ToList().ForEach(i => DeleteItem(i)),
    //                (items) => items != null && items.Count() > 0 && AllowDelete);
    //        }
    //        return _DeleteCommand;
    //    }
    //}
    //public void DeleteItem(DataObjectModel item)
    //{
    //        if (item == null) { throw new ArgumentNullException("item"); }

    //    myCollection.Remove(item.Object);
    //}



    //XAML:

    //<Button Command="{Binding DeleteCommand, ElementName=...}" CommandParameter="{Binding}">
    //    Remove item
    //</Button>


}
