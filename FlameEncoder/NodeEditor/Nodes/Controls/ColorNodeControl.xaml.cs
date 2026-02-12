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
using FlameEncoder.Data;

namespace FlameEncoder.NodeEditor.Nodes.Controls
{
    /// <summary>
    /// Interaction logic for ColorNodeControl.xaml
    /// </summary>
    /// 

    public partial class ColorNodeControl : UserControl
    {
        public UpdateDataDelegate<Color32> UpdateData;

        public ColorNodeControl()
        {
            InitializeComponent();
        }

        private void Grid_MouseDown(object sender, MouseButtonEventArgs e)
        {
            Color c = new Color();
            float Intensity = 0;
            if (ColorPicker.GetColor(out c, out Intensity))
            {
                SetColor(c, Intensity);
            }
        }
        public void SetColor(Color color, float Intensity)
        {
            bg.Background = new SolidColorBrush(color);
            UpdateData(new Color32(color.R / 255.0f * Intensity, color.G / 255.0f * Intensity, color.B / 255.0f * Intensity, color.A / 255.0f));
            
        }
    }
}
