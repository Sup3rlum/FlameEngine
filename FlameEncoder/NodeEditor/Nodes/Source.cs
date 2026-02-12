using System;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Controls;
using System.Drawing;
using SixLabors.ImageSharp;

using DPoint = System.Drawing.Point;
using WColor = System.Windows.Media.Color;
using WPoint = System.Windows.Point;

using FlameEncoder.Data;

using FlameEncoder.NodeEditor.Nodes.Controls;
using SixLabors.ImageSharp.Memory;
using SixLabors.ImageSharp.PixelFormats;

namespace FlameEncoder.NodeEditor.Nodes
{
    public class ColorNode : Node
    {
        public Color32 _color;
        public Color32 Color 
        { 
            get => _color; 
            set
            {
                _color = value;
                ((ColorNodeControl)Control).SetColor(Color32.Normalize(value).ToWPFColor(), value.Intensity);
            }
        }
        public override string NodeData => Color.ToString();

        public ColorNode(Color32 color) : base("Color", new ColorNodeControl(),Colors.Yellow)
        {
            ((ColorNodeControl)Control).UpdateData = (Color32 color) =>
            {
                this._color = color;
            };


            Color = color;
            AddSocket("Color", OutputNodeSocket.Create<Color32>(this,
                (location) =>
                {
            return Color;
        }));
        }
        public ColorNode() : this(new Color32(0,0,0,1))
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

            this.AddSocket("Color", OutputNodeSocket.Create<Color32>(this,
                (location) =>
                {

                    var img = Texture.TextureMap.Data[0];
                    var rgba = img[(int)(location.X * Texture.TextureMap.Data[0].Width), (int)(location.Y * Texture.TextureMap.Data[0].Height)];


                    return new Color32(rgba);   
                }));
        }
    }
}
