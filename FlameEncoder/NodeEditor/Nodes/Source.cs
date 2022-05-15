using System;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Controls;
using System.Drawing;
using SixLabors.ImageSharp;

using DPoint = System.Drawing.Point;
using DColor = System.Drawing.Color;
using WColor = System.Windows.Media.Color;
using WPoint = System.Windows.Point;

using FlameEncoder.NodeEditor.Nodes.Controls;

namespace FlameEncoder.NodeEditor.Nodes
{
    public class ColorNode : Node
    {
        public DColor _color;
        public DColor Color 
        { 
            get => _color; 
            set
            {
                _color = value;
                ((ColorNodeControl)Control).SetColor(WColor.FromArgb(value.A, value.R, value.G, value.B));
            }
        }
        public override string NodeData => Color.ToArgb().ToString();

        public ColorNode(DColor color) :
            base("Color",
                new ColorNodeControl(),
                Colors.Yellow)
        {
            ((ColorNodeControl)Control).UpdateData = (WColor color) =>
            {
                this._color = DColor.FromArgb(color.A, color.R, color.G, color.B);
            };


            this.Color = color;
            this.AddSocket("Color", OutputNodeSocket.Create<DColor>(this,
                (location) =>
                {
            return Color;
        }));
        }
        public ColorNode() : this(DColor.Black)
        {
        }


    }
    public class TextureNode : Node
    {
        public TextureView _texture;
        public TextureView Texture 
        {
            get => _texture;
            set
            {
                _texture = value;
                ((TextureNodeControl)Control).SetTexture(value);
            }
        }
        public override string NodeData => Texture == null ? "" : Texture.TextureMap.Path;

        public TextureNode() : base("Texture", new TextureNodeControl(), Colors.Yellow)
        {
            ((TextureNodeControl)Control).UpdateData = (TextureView texture) =>
            {
                this._texture = texture;
            };

            this.AddSocket("Color", OutputNodeSocket.Create<DColor>(this,
                (location) =>
                {
                    var rgba = Texture.TextureMap.Data[0][(int)(location.X * Texture.TextureMap.Data[0].Width), (int)(location.Y * Texture.TextureMap.Data[0].Height)];


                    int red = rgba.R / 256;
                    int green = rgba.G / 256;
                    int blue = rgba.B / 256;
                    int alpha = rgba.A / 256;

                    return DColor.FromArgb(alpha, red, green, blue);
                }));
        }
    }
}
