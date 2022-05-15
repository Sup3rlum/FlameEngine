using System;
using System.Windows.Media;
using System.Windows.Media.Imaging;

using System.Drawing;


using SixLabors.ImageSharp;

using DPoint = System.Drawing.Point;
using DColor = System.Drawing.Color;
using WColor = System.Windows.Media.Color;
using System.Windows.Controls;


using FlameEncoder.NodeEditor.Nodes.Controls;

namespace FlameEncoder.NodeEditor.Nodes
{
    public class ChannelSplitterNode : Node
    {
        public override string NodeData => "";

        public ChannelSplitterNode() : base("Channel Splitter", new ColorNodeControl(), Colors.Yellow)
        {
            AddSocket("Color", InputNodeSocket.Create<DColor>(this));

            AddSocket("Red", OutputNodeSocket.Create<DColor>(this, (location) =>
            {
                var color = (DColor)Inputs["Color"].Data(location);
                return DColor.FromArgb(255, color.R, color.R, color.R);
            }));

            AddSocket("Green", OutputNodeSocket.Create<DColor>(this, (location) =>
            {
                var color = (DColor)Inputs["Color"].Data(location);
                return DColor.FromArgb(255, color.G, color.G, color.G);
            }));

            AddSocket("Blue", OutputNodeSocket.Create<DColor>(this, (location) =>
            {
                var color = (DColor)Inputs["Color"].Data(location);
                return DColor.FromArgb(255, color.B, color.B, color.B);

            }));

            AddSocket("Alpha", OutputNodeSocket.Create<DColor>(this, (location) =>
            {
                var color = (DColor)Inputs["Color"].Data(location);
                return DColor.FromArgb(255, color.A, color.A, color.A);
            }));
        }

    }

    public class ChannelMixerNode : Node
    {
        public override string NodeData => "";


        public ChannelMixerNode() : base("Channel Mixer", new ColorNodeControl(), Colors.Yellow)
        {
            AddSocket("Red", InputNodeSocket.Create<DColor>(this));
            AddSocket("Green", InputNodeSocket.Create<DColor>(this));
            AddSocket("Blue", InputNodeSocket.Create<DColor>(this));
            AddSocket("Alpha", InputNodeSocket.Create<DColor>(this));

            AddSocket("Color", OutputNodeSocket.Create<DColor>(this,
                (location) =>
                {
                    var colorR = ((DColor)Inputs["Red"].Data(location)).R;
                    var colorG = ((DColor)Inputs["Green"].Data(location)).R;
                    var colorB = ((DColor)Inputs["Blue"].Data(location)).R;
                    var colorA = ((DColor)Inputs["Alpha"].Data(location)).R;

                    return DColor.FromArgb(colorA, colorR, colorG, colorG);
                }));
        }

    }

    public class DesaturateNode : Node
    {
        public override string NodeData => "";

        public DesaturateNode() : base("Desaturate", new ColorNodeControl(), Colors.Yellow)
        {
            AddSocket("Color", InputNodeSocket.Create<DColor>(this));

            AddSocket("Color", OutputNodeSocket.Create<DColor>(this,
                (location) =>
                {
                    var color = ((DColor)Inputs["Color"].Data(location));


                    float lum = (color.R / 255.0f) * 0.3f +
                                (color.G / 255.0f) * 0.6f +
                                (color.B / 255.0f) * 0.1f;

                    int colorLum = (int)(lum * 255.0f);

                    return DColor.FromArgb(255, colorLum, colorLum, colorLum);
                }));
        }
    }

    public class InvertNode : Node
    {
        public override string NodeData => "";

        public InvertNode() : base("Invert", new ColorNodeControl(), Colors.Yellow)
        {
            AddSocket("Color", InputNodeSocket.Create<DColor>(this));
            AddSocket("Color", OutputNodeSocket.Create<DColor>(this,
                (location) =>
                {
                    var color = ((DColor)Inputs["Color"].Data(location));

                    int invR = 255 - color.R;
                    int invG = 255 - color.G;
                    int invB = 255 - color.B;


                    return DColor.FromArgb(color.A, invR, invG, invB);
                }));
        }

    }

    public class HueNode : Node
    {
        public override string NodeData => "";

        public HueNode() : base("Hue", new ColorNodeControl(), Colors.Yellow)
        {
            AddSocket("Color", InputNodeSocket.Create<DColor>(this));
            AddSocket("Color", OutputNodeSocket.Create<DColor>(this,
                (location) =>
                {
                    var color = ((DColor)Inputs["Color"].Data(location));

                    int invR = 255 - color.R;
                    int invG = 255 - color.G;
                    int invB = 255 - color.B;


                    return DColor.FromArgb(color.A, invR, invG, invB);
                }));
        }
    }
}
