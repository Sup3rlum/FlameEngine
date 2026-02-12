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

namespace FlameEncoder.NodeEditor.Nodes.Controls
{
    /// <summary>
    /// Interaction logic for EnvironmentMapNodeControl.xaml
    /// </summary>
    /// 
    public struct EnvironmentMapProperties
    {
        public int Width;
    }

    public partial class EnvironmentMapNodeControl : UserControl
    {

        EnvironmentMapProperties environmentMapProperties = new EnvironmentMapProperties();
        public UpdateDataDelegate<EnvironmentMapProperties> UpdateData;


        public EnvironmentMapNodeControl()
        {
            InitializeComponent();
        }

        private void widthBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            int newWidth = 0;
            if (int.TryParse(widthBox.Text, out newWidth))
            {
                environmentMapProperties.Width = newWidth;
            }

            if (UpdateData != null)
            {
                UpdateData(environmentMapProperties);
            }
        }

        private void widthBox_MouseDown(object sender, MouseButtonEventArgs e)
        {
            widthBox.Focus();
        }


        private void widthBox_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            int a;
            e.Handled = !int.TryParse(e.Text, out a);
        }


        public void SetMaterialProps(EnvironmentMapProperties props)
        {
            environmentMapProperties = props;

            widthBox.Text = $"{props.Width}";
        }
    }
}
