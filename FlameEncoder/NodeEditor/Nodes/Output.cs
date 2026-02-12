using System;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Controls;
using System.Drawing;


using SixLabors.ImageSharp;

using DPoint = System.Drawing.Point;
using WColor = System.Windows.Media.Color;
using WPoint = System.Windows.Point;

using FlameEncoder.NodeEditor.Nodes.Controls;
using FlameEncoder.Data;

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

            AddSocket("Diffuse", InputNodeSocket.Create<Color32>(this));
            AddSocket("Normal", InputNodeSocket.Create<Color32>(this));
            AddSocket("Height", InputNodeSocket.Create<Color32>(this));
            AddSocket("Roughness", InputNodeSocket.Create<Color32>(this));
            AddSocket("Metallic", InputNodeSocket.Create<Color32>(this));
            AddSocket("AO", InputNodeSocket.Create<Color32>(this));
            AddSocket("Emissive", InputNodeSocket.Create<Color32>(this));
        }
        public MaterialNode() : this(new MaterialProperties() { Width = 1024, Height = 1024})
        {

        }

    }

    public class EnvironmentMapNode : Node
    {
        public EnvironmentMapProperties properties;

        public override string NodeData => $"{properties.Width}";

        public EnvironmentMapNode(int Width) : base("Environment Map", new EnvironmentMapNodeControl(), Colors.Yellow)
        {

            EnvironmentMapProperties props = new EnvironmentMapProperties();
            props.Width = Width;

            (Control as EnvironmentMapNodeControl).UpdateData = (EnvironmentMapProperties prop) =>
            {
                properties = prop;
            };

            (Control as EnvironmentMapNodeControl).SetMaterialProps(props);

            AddSocket("Specular", InputNodeSocket.Create<Color32>(this));
            AddSocket("Irradiance", InputNodeSocket.Create<Color32>(this));
        }
        public EnvironmentMapNode() : this(1024)
        {

        }
    }
}
