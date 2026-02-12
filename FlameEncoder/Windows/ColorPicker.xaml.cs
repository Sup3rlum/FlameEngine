using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Threading;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace FlameEncoder.Windows
{
    /// <summary>
    /// Interaction logic for ColorPicker.xaml
    /// </summary>
    public partial class ColorPicker : Window, INotifyPropertyChanged
    {
        public ColorPicker()
        {
            InitializeComponent();
        }


        float Hue = 100.0f;
        float Saturation = 1.0f;
        float Value = 1.0f;
        float Intensity = 1.0f;


        bool isSliderMoving = false;
        bool isColorMoving = false;
        bool isColorReady = false;


        Point _palettePoint;
        public Point palettePoint
        {
            get { return _palettePoint; }
            set
            {
                _palettePoint = value;

                //NotifyPropertyChanged("PaletteTop");
                //NotifyPropertyChanged("PaletteLeft");

            }
        }

        public int PaletteTop => (int)palettePoint.Y;
        public int PaletteLeft => (int)palettePoint.X;


        public event PropertyChangedEventHandler? PropertyChanged;

        public (int, int, int) HSVtoRGB(float h, float s, float v)
        {
            float C = v * s;
            float X = C * (1 - Math.Abs((h / 60.0f) % 2 - 1));
            float m = v - C;

            (float r, float g, float b) = h switch
            {
                (>= 0) and (< 60.0f) => (C, X, 0.0f),
                (>= 60.0f) and (< 120.0f) => (X, C, 0.0f),
                (>= 120.0f) and (< 180.0f) => (0.0f, C, X),
                (>= 180.0f) and (< 240.0f) => (0.0f, X, C),
                (>= 240.0f) and (< 300.0f) => (X, 0.0f, C),
                (>= 300.0f) and (< 360.0f) => (C, 0.0f, X)
            };

            return ((int)((r + m) * 255), (int)((g + m) * 255), (int)((b + m) * 255));
        }

        public string RGBtoHex(int r, int g, int b)
        {
            return $"#{r.ToString("X")}{g.ToString("X")}{b.ToString("X")}";
        }

        public delegate (int, int, int) WriteFunc(int x, int y);

        public void WritePixelsToBmp(WriteableBitmap bmp, WriteFunc writeFunc)
        {
            try
            {
                // Reserve the back buffer for updates.
                bmp.Lock();

                unsafe
                {
                    for (int x = 0; x < bmp.Width; x++)
                    {
                        for (int y = 0; y < bmp.Height; y++)
                        {
                            // Get a pointer to the back buffer.
                            IntPtr pBackBuffer = bmp.BackBuffer;

                            // Find the address of the pixel to draw.
                            pBackBuffer += y * bmp.BackBufferStride;
                            pBackBuffer += x * 4;

                            (int R, int G, int B) = writeFunc(x, y);

                            // Compute the pixel's color.
                            int color_data = R << 16; // R
                            color_data |= G << 8;   // G
                            color_data |= B << 0;   // B

                            // Assign the color data to the pixel.
                            *((int*)pBackBuffer) = color_data;

                            // Specify the area of the bitmap that changed.
                        }
                    }
                    bmp.AddDirtyRect(new Int32Rect(0, 0, (int)bmp.Width, (int)bmp.Height));
                }

            }
            finally
            {
                // Release the back buffer and make it available for display.
                bmp.Unlock();
            }
        }

        public void GenerateHueSlider()
        {
            WriteableBitmap bmp = new WriteableBitmap((int)hueSlider.Width, (int)hueSlider.Height, 96, 96, PixelFormats.Bgr32, null);
            WritePixelsToBmp(bmp, (x, y) => HSVtoRGB(360.0f * (float)(y / hueSlider.Height), 1.0f, 1.0f));
            hueSlider.Source = bmp;
        }


        protected void NotifyPropertyChanged([CallerMemberName] String propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public void SetCanvasHueSlice()
        {
            WriteableBitmap bmp = new WriteableBitmap((int)hsvPalette.Width, (int)hsvPalette.Height, 96, 96, PixelFormats.Bgr32, null);
            WritePixelsToBmp(bmp, (x, y) => HSVtoRGB(Hue, (float)(x / hsvPalette.Width), 1.0f - (float)(y / hsvPalette.Height)));
            hsvPalette.Source = bmp;
        }

        public void SetPreviewColor()
        {
            (int R, int G, int B) = HSVtoRGB(Hue, Saturation, Value);

            textBox_Red.Text = R.ToString();
            textBox_Green.Text = G.ToString();
            textBox_Blue.Text = B.ToString();

            textBox_Hue.Text = Hue.ToString();
            textBox_Saturation.Text = Saturation.ToString();
            textBox_Value.Text = Value.ToString();

            textBox_Hex.Text = RGBtoHex(R, G, B);

            WriteableBitmap bmp = new WriteableBitmap(2, 2, 96, 96, PixelFormats.Bgr32, null);
            WritePixelsToBmp(bmp, (x, y) =>
            {
                if (y == 0)
                {
                    return (Math.Min((int)(R*Intensity), 255), Math.Min((int)(G*Intensity), 255), Math.Min((int)(B*Intensity), 255));
                }
                else
                {
                    return (R, G, B);
                }
            });
            hsvPreview.Source = bmp;
        }

        private void Canvas_Loaded(object sender, RoutedEventArgs e)
        {
            SetCanvasHueSlice();
            GenerateHueSlider();
        }

        private void hsvPalette_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            isColorMoving = true;
        }

        private void hueSlider_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            isSliderMoving = true;
        }

        private void hsvPalette_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            isColorMoving = false;
        }

        private void hueSlider_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            isSliderMoving = false;
        }

        private void hsvPalette_MouseMove(object sender, MouseEventArgs e)
        {
            if (!isColorMoving)
                return;

            palettePoint = Mouse.GetPosition(hsvPalette);


            Saturation = (float)(palettePoint.X / hsvPalette.Width);
            Value = 1.0f - (float)(palettePoint.Y / hsvPalette.Width);

            SetPreviewColor();
        }

        private void hueSlider_MouseMove(object sender, MouseEventArgs e)
        {
            if (!isSliderMoving)
                return;

            Point p = Mouse.GetPosition(hueSlider);

            Hue = (float)(p.Y / hueSlider.Height) * 360;

            SetCanvasHueSlice();
            SetPreviewColor();
        }

        private void hueSlider_MouseLeave(object sender, MouseEventArgs e)
        {
            isSliderMoving = false;
            isColorMoving = false;
        }

        private void hsvPalette_MouseLeave(object sender, MouseEventArgs e)
        {
            isSliderMoving = false;
            isColorMoving = false;
        }

        private void acceptBtn_Click(object sender, RoutedEventArgs e)
        {
            isColorReady = true;
            Close();
        }

        private void cancelBtn_Click(object sender, RoutedEventArgs e)
        {
            isColorReady = false;
            Close();
        }

        public static bool GetColor(out Color color, out float Intensity)
        {
            var picker = new ColorPicker();
            picker.ShowDialog();

            if (picker.isColorReady)
            {
                (int r, int g, int b) = picker.HSVtoRGB(picker.Hue, picker.Saturation, picker.Value);

                color = Color.FromRgb((byte)r, (byte)g, (byte)b);
                Intensity = picker.Intensity;
                return true;
            }
            else
            {
                color = new Color();
                Intensity = 0;
                return false;
            }
        }

        private void intensitySlider_ValueChanged(object sender, RoutedPropertyChangedEventArgs<double> e)
        {
            float fVal = (float)intensitySlider.Value;

            if (fVal < 50.0f)
            {
                Intensity = 0.02f * fVal;
            }
            else
            {
                Intensity = MathF.Exp(0.1f * (fVal - 50.0f));
            }

            textBox_Intensity.Text = Intensity.ToString();

            SetPreviewColor();
        }
    }
}
