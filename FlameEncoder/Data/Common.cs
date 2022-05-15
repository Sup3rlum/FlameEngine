using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
using System.Linq;
using System.Runtime.InteropServices;
using System.Drawing;

using FlameEncoder.Compilers;


using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Formats;
using SixLabors.ImageSharp.PixelFormats;

namespace FlameEncoder.Data
{
    public class TextureMap
    {
        public string Path { get; set; }
        public List<Image<Rgba64>> Data { get; set; }
    }

    public abstract class TextureMapCollection
    {
        public Dictionary<string, TextureMap> Maps { get; set; }
        public ObservableCollection<TextureMap> Values
        {
            get; set;
        }

        public TextureMapCollection()
        {
            Maps = new Dictionary<string, TextureMap>();
            Values = new ObservableCollection<TextureMap>();

        }

        public void AddMap(string name, TextureMap map)
        {
            Maps.Add(name, map);
            Values.Clear();

            foreach (var (t, value) in Maps)
            {
                Values.Add(value);
            }
        }
        public abstract ITaskCompiler CreateCompiler();

    }
}
