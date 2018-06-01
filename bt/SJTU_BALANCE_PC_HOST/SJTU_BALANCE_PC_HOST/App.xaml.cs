using System;
using System.Windows;

namespace SJTU_BALANCE_PC_HOST
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App : Application
    {
        /// <summary>
        /// Entry point of the program
        /// </summary>
        [STAThread]
        static void Main()
        {
            var application = new Application();
            var view = new MainWindow();
            var controller = new Controller(view);
            var processor = new Processor(view);

            application.Run(view);
        }
    }
}
