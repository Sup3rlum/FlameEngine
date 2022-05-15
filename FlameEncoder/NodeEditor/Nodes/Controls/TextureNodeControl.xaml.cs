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
    /// Interaction logic for TextureNodeControl.xaml
    /// </summary>
    public partial class TextureNodeControl : UserControl
    {
        public UpdateDataDelegate<TextureView> UpdateData;

        public TextureNodeControl()
        {
            InitializeComponent();
        }

        private void Image_MouseDown(object sender, MouseButtonEventArgs e)
        {
            var newTex = new NewTexture();
            if (newTex.ShowDialog().HasValue && newTex.textureView != null)
            {
                UpdateData(newTex.textureView);
                SetTexture(newTex.textureView);
            }

        }
        public void SetTexture(TextureView textureView)
        {
            this.bgImage.Source = textureView.Image;
        }
    }
}
