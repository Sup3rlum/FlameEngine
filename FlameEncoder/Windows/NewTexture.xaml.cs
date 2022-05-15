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
using System.IO;
using System.Drawing;
using System.Drawing.Imaging;

using Microsoft.Win32;

using FlameEncoder.Data;


using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Formats;
using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp.Processing;

namespace FlameEncoder
{
    /// <summary>
    /// Interaction logic for NewTexture.xaml
    /// </summary>
    public partial class NewTexture : Window
    {
        public NewTexture()
        {
            InitializeComponent();
        }


        public TextureView textureView { get; set; }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();

            openFileDialog.Filter = "Image Files (*.jpg, *.png, *.tif, *.bmp, *.tga)|*.jpg, *.png, *.tif, *.bmp, *.tga| PNG Images (*.png)|*.png| JPEG images (*.jpg)|*.jpg";
            if (openFileDialog.ShowDialog() == true)
            {
                Image<Rgba64> im = Image<Rgba64>.Load(openFileDialog.FileName).CloneAs<Rgba64>();
                pathBox.Text = openFileDialog.FileName;

                textureView = new TextureView(new TextureMap()
                {
                    Data = new List<Image<Rgba64>>() { im }
                });


                var bitmapImage = new BitmapImage();
                using (var ms = new MemoryStream())
                {
                    im.SaveAsBmp(ms);
                    ms.Seek(0, SeekOrigin.Begin);

                    bitmapImage.BeginInit();
                    bitmapImage.CacheOption = BitmapCacheOption.OnLoad;
                    bitmapImage.StreamSource = ms;
                    bitmapImage.EndInit();
                }

                previewImage.Source = bitmapImage;
            }
        }

        private void Import_Click(object sender, RoutedEventArgs e)
        {
            if (textureView == null || pathBox.Text == "")
                return;

            textureView.TextureMap.Path = pathBox.Text;
            Close();
        }
        private void Cancel_Click(object sender, RoutedEventArgs e)
        {
            Close();
        }
    }
}
