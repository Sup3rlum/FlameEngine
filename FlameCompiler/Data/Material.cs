using System;
using System.Collections.Generic;
using System.Text;
using SixLabors.ImageSharp;
using SixLabors.ImageSharp.ColorSpaces;
using SixLabors.ImageSharp.Formats;
using SixLabors.ImageSharp.PixelFormats;

namespace FlameCompiler.Data
{

    public enum MaterialMapType : int
    {
        Diffuse = 0,
        Normal = 1,
        Height = 2,
        Roughness = 3,
        Metallic = 4,
        AmbientOcclusion = 5
    }


public class Material
    {
        public Image<Rgba32>[] mapArray;

        public Material()
        {
            mapArray = new Image<Rgba32>[6];
        }
    }
}
