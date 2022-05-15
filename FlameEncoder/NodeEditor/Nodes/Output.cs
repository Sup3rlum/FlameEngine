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

    public class MaterialNode : Node
    {
        public MaterialProperties properties;

        public override string NodeData => $"{properties.Width}:{properties.Height}";

        public MaterialNode(MaterialProperties props) : base("Material", new MaterialNodeControl(), Colors.Yellow)
        {
            (Control as MaterialNodeControl).UpdateData = (MaterialProperties prop) =>
            {
                properties = prop;
            };

            (Control as MaterialNodeControl).SetMaterialProps(props);

            AddSocket("Diffuse", InputNodeSocket.Create<DColor>(this));
            AddSocket("Normal", InputNodeSocket.Create<DColor>(this));
            AddSocket("Height", InputNodeSocket.Create<DColor>(this));
            AddSocket("Roughness", InputNodeSocket.Create<DColor>(this));
            AddSocket("Metallic", InputNodeSocket.Create<DColor>(this));
            AddSocket("AO", InputNodeSocket.Create<DColor>(this));
            AddSocket("Emissive", InputNodeSocket.Create<DColor>(this));
        }
        public MaterialNode() : this(new MaterialProperties() { Width = 1024, Height = 1024})
        {

        }

    }

    public class EnvironmentMapNode : Node
    {
        public override string NodeData => "";

        public EnvironmentMapNode() : base("Environment Map", new ColorNodeControl(), Colors.Yellow)
        {
            AddSocket("Specular", InputNodeSocket.Create<DColor>(this));
            AddSocket("Irradiance", InputNodeSocket.Create<DColor>(this));
        }
    }
}
