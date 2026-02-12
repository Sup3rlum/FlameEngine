using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

using FlameEditorCLR;

namespace FlameEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        EditorApp editorApp;

        public MainWindow()
        {
            System.Windows.Forms.Integration.ElementHost.EnableModelessKeyboardInterop(this);

            InitializeComponent();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            editorApp = new EditorApp(enginePanel.Handle, enginePanel.Width, enginePanel.Height);

            while (editorApp.IsContextActive())
            {
                editorApp.Frame();
                System.Windows.Forms.Application.DoEvents();
            }
        }

        private void Window_Closed(object sender, EventArgs e)
        {
            editorApp.PollClose();
        }
    }
}
