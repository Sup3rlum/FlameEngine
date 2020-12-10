using System;
using System.Collections.Generic;
using System.Text;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.ColorSpaces;
using SixLabors.ImageSharp.Formats;
using SixLabors.ImageSharp.PixelFormats;

namespace FlameCompiler.Data
{
    public class Material
    {
        public Dictionary<string, Image<Rgba32>> mapDictionary;

        public Material()
        {
            mapDictionary = new Dictionary<string, Image<Rgba32>>();
        }
    }
}
