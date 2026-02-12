using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;

using SixLabors.ImageSharp;
using SixLabors.ImageSharp.PixelFormats;

using SharpEXR;
using System.Windows.Media;

namespace FlameEncoder.ImportScripts
{

    public enum PixelBithdepth
    {
        Bits8,
        Bits16,
        Bits32
    }

    public static class SourceImage
    {

        public static Image<RgbaVector> LoadImage(string path)
        {
            if (path == "")
                return null;

            var ext = System.IO.Path.GetExtension(path);

            Image<RgbaVector> im;

            if (ext == ".exr")
            {
                var file = EXRFile.FromFile(path);
                var part = file.Parts[0];

                part.OpenParallel(path);

                var values = part.GetFloats(ChannelConfiguration.RGB, false, GammaEncoding.Linear);
                float[] halfs = new float[4 * (values.Length / 3)];

                for (int i = 0; i < halfs.Length / 4; i++)
                {
                    halfs[i * 4] = values[i * 3];
                    halfs[i * 4 + 1] = values[i * 3 + 1];
                    halfs[i * 4 + 2] = values[i * 3 + 2];
                    halfs[i * 4 + 3] = 1.0f;
                }

                var isharpPixels = MemoryMarshal.Cast<float, byte>(halfs);
                im = Image.LoadPixelData<RgbaVector>(isharpPixels, part.DataWindow.Width, part.DataWindow.Height);

            }
            else
            {
                im = Image.Load<RgbaVector>(path);
            }


            return im;
        }
    }
}
