using System;
using System.Windows.Media;
using System.Windows.Media.Imaging;

using System.Drawing;


using SixLabors.ImageSharp;

using DPoint = System.Drawing.Point;
using DColor = System.Drawing.Color;
using WColor = System.Windows.Media.Color;
using System.Windows.Controls;
using FlameEncoder.Data;


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
                var color = (Color32)Inputs["Color"].Data(location);
                return new Color32(color.R, color.R, color.R, 1.0f);
            }));

            AddSocket("Green", OutputNodeSocket.Create<DColor>(this, (location) =>
            {
                var color = (Color32)Inputs["Color"].Data(location);
                return new Color32(color.G, color.G, color.G, 1.0f);
            }));

            AddSocket("Blue", OutputNodeSocket.Create<DColor>(this, (location) =>
            {
                var color = (Color32)Inputs["Color"].Data(location);
                return new Color32(color.B, color.B, color.B, 1.0f);

            }));

            AddSocket("Alpha", OutputNodeSocket.Create<DColor>(this, (location) =>
            {
                var color = (Color32)Inputs["Color"].Data(location);
                return new Color32(color.A, color.A, color.A, 1.0f);
            }));
        }

    }

    public class ChannelMixerNode : Node
    {
        public override string NodeData => "";


        public ChannelMixerNode() : base("Channel Mixer", new ColorNodeControl(), Colors.Yellow)
        {
            AddSocket("Red", InputNodeSocket.Create<Color32>(this));
            AddSocket("Green", InputNodeSocket.Create<Color32>(this));
            AddSocket("Blue", InputNodeSocket.Create<Color32>(this));
            AddSocket("Alpha", InputNodeSocket.Create<Color32>(this));

            AddSocket("Color", OutputNodeSocket.Create<Color32>(this,
                (location) =>
                {
                    var colorR = ((Color32)Inputs["Red"].Data(location)).R;
                    var colorG = ((Color32)Inputs["Green"].Data(location)).R;
                    var colorB = ((Color32)Inputs["Blue"].Data(location)).R;
                    var colorA = ((Color32)Inputs["Alpha"].Data(location)).R;

                    return new Color32(colorR, colorG, colorB, colorA);
                }));
        }

    }

    public class DesaturateNode : Node
    {
        public override string NodeData => "";

        public DesaturateNode() : base("Desaturate", new ColorNodeControl(), Colors.Yellow)
        {
            AddSocket("Color", InputNodeSocket.Create<Color32>(this));

            AddSocket("Color", OutputNodeSocket.Create<Color32>(this,
                (location) =>
                {
                    var color = ((Color32)Inputs["Color"].Data(location));


                    float lum = color.R * 0.3f +
                                color.G * 0.6f +
                                color.B * 0.1f;


                    return new Color32(lum, lum, lum, 1.0f);
                }));
        }
    }

    public class InvertNode : Node
    {
        public override string NodeData => "";

        public InvertNode() : base("Invert", new ColorNodeControl(), Colors.Yellow)
        {
            AddSocket("Color", InputNodeSocket.Create<Color32>(this));
            AddSocket("Color", OutputNodeSocket.Create<Color32>(this,
                (location) =>
                {
                    var color = ((Color32)Inputs["Color"].Data(location));

                    float invR = 1.0f - color.R;
                    float invG = 1.0f - color.G;
                    float invB = 1.0f - color.B;


                    return new Color32(invR, invG, invB, color.A);
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
