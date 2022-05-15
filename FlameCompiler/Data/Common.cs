using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
using System.Linq;
using System.Runtime.InteropServices;
using System.Drawing;

using FlameCompiler.Compilers;


using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Formats;
using SixLabors.ImageSharp.PixelFormats;

namespace FlameCompiler.Data
{
    public class TextureMap
    {
        public string Name { get; set; }
        public string Path { get; set; }
        public List<Image<Rgba64>> Data { get; set; }
    }

    public interface ITextureMapCollection
    {
        public ObservableCollection<TextureMap> Values { get; }
        public string TypeName { get; }

        public void AddMap(string name, TextureMap map);
        public ITaskCompiler GetCompiler();
    }

}
