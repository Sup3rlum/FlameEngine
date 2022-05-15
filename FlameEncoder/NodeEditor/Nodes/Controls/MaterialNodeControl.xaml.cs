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
    /// Interaction logic for MaterialNodeControl.xaml
    /// </summary>
    /// 

    public struct MaterialProperties
    {
        public int Width;
        public int Height;
    }

    public partial class MaterialNodeControl : UserControl
    {
        public UpdateDataDelegate<MaterialProperties> UpdateData;

        MaterialProperties materialProperties = new MaterialProperties();

        public MaterialNodeControl()
        {
            InitializeComponent();
        }

        private void widthBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            int newWidth = 0;
            if (int.TryParse(widthBox.Text, out newWidth))
            {
                materialProperties.Width = newWidth;
            }

            if (UpdateData != null)
            {
                UpdateData(materialProperties);
            }
        }

        private void heightBox_TextChanged(object sender, TextChangedEventArgs e)
        {
            int newHeight = 0;
            if (int.TryParse(widthBox.Text, out newHeight))
            {
                materialProperties.Height = newHeight;
            }

            if (UpdateData != null)
            {
                UpdateData(materialProperties);
            }
        }

        private void widthBox_MouseDown(object sender, MouseButtonEventArgs e)
        {
            widthBox.Focus();
        }

        private void heightBox_MouseDown(object sender, MouseButtonEventArgs e)
        {
            heightBox.Focus();

        }

        private void widthBox_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            int a;
            e.Handled = !int.TryParse(e.Text, out a);
        }

        private void heightBox_PreviewTextInput(object sender, TextCompositionEventArgs e)
        {
            int a;
            e.Handled = !int.TryParse(e.Text, out a);
        }

        public void SetMaterialProps(MaterialProperties props)
        {
            materialProperties = props;

            widthBox.Text = $"{ props.Width }";
            heightBox.Text = $"{ props.Height }";

        }

    }
}
