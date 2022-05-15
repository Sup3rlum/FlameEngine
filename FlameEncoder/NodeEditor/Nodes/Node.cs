using System;
using System.Collections.Generic;
using System.Linq;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Controls;

using System.IO;
using System.Xml.Linq;

using System.ComponentModel;
using System.Runtime.CompilerServices;


using DPoint = System.Drawing.Point;
using DColor = System.Drawing.Color;


namespace FlameEncoder.NodeEditor.Nodes
{
    public delegate void UpdateDataDelegate<T>(T data);

    public abstract class Node : INotifyPropertyChanged
    {
        public Dictionary<string, InputNodeSocket> Inputs = new Dictionary<string, InputNodeSocket>();
        public Dictionary<string, OutputNodeSocket> Outputs = new Dictionary<string, OutputNodeSocket>();
        public NodeGraph NodeManager;
        public int ID;
        public int _isSelected = 0;
        public int IsSelected
        {
            get => _isSelected;
            set
            {
                _isSelected = value;
                NotifyPropertyChanged("IsSelected");
            }
        }

        public string Name { get; protected set; }
        public abstract string NodeData { get; }
        public Color TagColor { get; protected set; }
        public UserControl Control { get; protected set; }
        public double NodeHeight
        {
            get
            {
                if (Control == null) return 0;
                else return Control.Height;
            }
        }

        public System.Windows.Point _position;
        public System.Windows.Point Position
        {
            get
            {
                return _position;
            }
            set
            {
                _position = value;

                NotifyPropertyChanged("Top");
                NotifyPropertyChanged("Left");


                foreach (var inp in Inputs)
                {
                    foreach (NodeConnection conn in inp.Value.Connections)
                    {
                        conn.NotifyPropertyChanged("SvgString");
                    }
                }
                foreach (var outp in Outputs)
                {
                    foreach (NodeConnection conn in outp.Value.Connections)
                    {
                        conn.NotifyPropertyChanged("SvgString");
                    }
                }
            }
        }

        public int Top { get => (int)Position.Y; }
        public int Left { get => (int)Position.X; }

        public List<KeyValuePair<string, InputNodeSocket>> InputsView => Inputs.ToList();
        public List<KeyValuePair<string, OutputNodeSocket>> OutputsView => Outputs.ToList();

        public event PropertyChangedEventHandler? PropertyChanged;

        protected void NotifyPropertyChanged([CallerMemberName] String propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        protected void AddSocket(string name, InputNodeSocket socket)
        {
            Inputs[name] = socket;
            Inputs[name].Name = name;
        }
        protected void AddSocket(string name, OutputNodeSocket socket)
        {
            Outputs[name] = socket;
            Outputs[name].Name = name;
        }

        public static ImageSource EmptyPreview()
        {
            System.Windows.Media.PixelFormat pf = PixelFormats.Rgb24;
            int width = 1;
            int height = 1;
            int rawStride = (width * pf.BitsPerPixel + 7) / 8;
            byte[] rawImage = new byte[rawStride * height];


            // Create a BitmapSource.
            BitmapSource bitmap = BitmapSource.Create(width, height,
                96, 96, pf, null,
                rawImage, rawStride);


            return bitmap;
        }

        protected Node(string Name, UserControl displayControl, Color TagColor)
        {
            this.Name = Name;
            this.Control = displayControl;
            this.TagColor = TagColor;
        }
    }
}
