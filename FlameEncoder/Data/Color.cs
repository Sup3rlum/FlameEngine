using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using WColor = System.Windows.Media.Color;

using SixLabors.ImageSharp.PixelFormats;

namespace FlameEncoder.Data
{
    public struct Color32
    {
        public float R;
        public float G;
        public float B;
        public float A;


        public Color32(float r, float g, float b, float a)
        {
            R = r;
            G = g;
            B = b;
            A = a;
        }

        public Color32(RgbaVector vec)
        {
            R = vec.R;
            G = vec.G;
            B = vec.B;
            A = vec.A;
        }

        public WColor ToWPFColor()
        {
            return WColor.FromArgb((byte)(A * 255), (byte)(R * 255), (byte)(G * 255), (byte)(B * 255));
        }

        public static Color32 Normalize(Color32 color)
        {
            return new Color32(color.R / color.Intensity, color.G / color.Intensity, color.B / color.Intensity, color.A);
        }

        public float Intensity => MathF.Sqrt(R * R + G * G + B * B);

        public override string ToString()
        {
            return $"{R};{G};{B};{A}";
        }

        public static Color32 FromString(string str)
        {
            string[] vals = str.Split(';');

            float r = float.Parse(vals[0]);
            float g = float.Parse(vals[1]);
            float b = float.Parse(vals[2]);
            float a = float.Parse(vals[3]);

            return new Color32(r, g, b, a);
        }

    }

    public struct Color8
    {
        public byte R;
        public byte G;
        public byte B;
        public byte A;

        public Color8(byte r, byte g, byte b, byte a)
        {
            R = r;
            G = g;
            B = b;
            A = a;
        }

    }
}
