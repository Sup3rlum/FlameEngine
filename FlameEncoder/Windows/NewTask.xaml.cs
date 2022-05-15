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
using System.Windows.Shapes;

using FlameEncoder.ImportScripts;
using FlameEncoder.Data;
using FlameEncoder.NodeEditor;

namespace FlameEncoder
{
    /// <summary>
    /// Interaction logic for NewTask.xaml
    /// </summary>
    public partial class NewTask : Window
    {
        public NewTask()
        {
            InitializeComponent();
        }


        public NodeGraph Graph { get; set; }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            if (taskNameBox.Text == "" || outputBox.Text == "")
            {
                return;
            }


            Graph = new NodeGraph()
            {
                Name = taskNameBox.Text,
                OutputFileName = outputBox.Text      
            };

            this.Close();
        }
    }
}
