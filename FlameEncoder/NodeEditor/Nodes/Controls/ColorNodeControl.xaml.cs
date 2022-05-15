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

using FlameEncoder.Windows;

namespace FlameEncoder.NodeEditor.Nodes.Controls
{
    /// <summary>
    /// Interaction logic for ColorNodeControl.xaml
    /// </summary>
    /// 

    public partial class ColorNodeControl : UserControl
    {
        public UpdateDataDelegate<Color> UpdateData;

        public ColorNodeControl()
        {
            InitializeComponent();
        }

        private void Grid_MouseDown(object sender, MouseButtonEventArgs e)
        {
            Color c = new Color();
            if (ColorPicker.GetColor(out c))
            {
                SetColor(c);
            }
        }
        public void SetColor(Color color)
        {
            bg.Background = new SolidColorBrush(color);
            UpdateData(color);
            
        }
    }
}
