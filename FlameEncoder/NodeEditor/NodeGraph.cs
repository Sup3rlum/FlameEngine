

using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Runtime.InteropServices;
using System.Drawing.Imaging;
using System.IO;
using System.Xml.Linq;

using System.ComponentModel;
using System.Runtime.CompilerServices;


using Microsoft.Win32;

using FlameEncoder.ImportScripts;
using FlameEncoder.Compilers;
using FlameEncoder.Data;

//using FlameEncoderCLR;

using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Formats;
using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp.Processing;


using DPoint = System.Drawing.Point;
using DColor = System.Drawing.Color;


using FlameEncoder.NodeEditor.Nodes;
using System.Windows.Media.Media3D;

using SharpEXR;

namespace FlameEncoder.NodeEditor
{

    public delegate object DataTranform(PixelPosition location);

    

    public class NodeConnection : INotifyPropertyChanged
    {
        public OutputNodeSocket Output;
        public InputNodeSocket Input;

        public DPoint Start => new DPoint(Output.CanvasPosition.X + 4, Output.CanvasPosition.Y + 4);
        public DPoint Anchor1 => new DPoint(Start.X + (End.X - Start.X) / 2, Start.Y);
        public DPoint Anchor2 => new DPoint(End.X - (End.X - Start.X) / 2, End.Y);
        public DPoint End => new DPoint(Input.CanvasPosition.X + 4, Input.CanvasPosition.Y + 4);

        public event PropertyChangedEventHandler? PropertyChanged;

        public void NotifyPropertyChanged([CallerMemberName] String propertyName = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public string SvgString
        {
            get
            {
                return $"M {Start.X}, {Start.Y} C {Anchor1.X}, {Anchor1.Y} {Anchor2.X}, {Anchor2.Y} {End.X}, {End.Y}";
            }
        }

        public NodeConnection(InputNodeSocket input, OutputNodeSocket output)
        {
            Output = output;
            Input = input;

            Output.Connections.Add(this);
            Input.Connections.Add(this);

        }

        public NodeConnection()
        {

        }

    }

    public class NodeSocket
    {
        public Type SocketType;
        public List<NodeConnection> Connections = new List<NodeConnection>();
        public Node Parent;
        public DPoint Position { get; set; }
        public DPoint CanvasPosition => new ((int)Parent.Position.X + Position.X, (int)Parent.Position.Y + Position.Y);
        public string Name { get; set; }

    }

    public struct PixelPosition
    {
        public double X;
        public double Y;
        public int Layer;

        public PixelPosition(double x, double y, int layer=0)
        {
            X = x;
            Y = y;
            Layer = layer;
        }

    }

    public class InputNodeSocket : NodeSocket
    {
        public object Default;

        public object Data(PixelPosition location)
        {
            if (Connections.Count == 0)
            {
                return Default;
            }
            else
            {
                return Connections[0].Output.Tranform(location);
            }
        }

        public static InputNodeSocket Create<T>(Node node) where T : struct
        {
            InputNodeSocket socket = new InputNodeSocket()
            {
                SocketType = typeof(T),
                Default = new T(),
                Parent = node
            };
            return socket;
        }
    }

    public class OutputNodeSocket : NodeSocket
    {
        public DataTranform Tranform;

        public static OutputNodeSocket Create<T>(Node node, DataTranform transform) where T : struct
        {
            OutputNodeSocket socket = new OutputNodeSocket()
            {
                SocketType = typeof(T),
                Parent = node,
                Tranform = transform
            };

            return socket;
        }

    }

    public class NodeGraph
    {
        public string[] FaceNames =
        {
            "+X",
            "-X",
            "+Y",
            "-Y",
            "+Z",
            "-Z"
        };


        public string Name { get; set; }
        public string OutputFileName { get; set; }
        public ObservableCollection<Node> Nodes { get; set; }
        public ObservableCollection<NodeConnection> Connections { get; set; }
        public Node Output;

        public int IdCounter = 0;

        public NodeGraph()
        {
            Nodes = new ObservableCollection<Node>();
            Connections = new ObservableCollection<NodeConnection>();
        }


        public TNode CreateNode<TNode>() where TNode : Node, new()
        {
            var node = new TNode()
            {
                NodeManager = this,
                ID = IdCounter++
            };
            Nodes.Add(node);
            return node;
        }

        public void DeleteNode(Node node)
        {
            foreach (var (name, socket) in node.Inputs)
            {
                foreach (var connection in socket.Connections)
                {
                    connection.Output.Connections.Remove(connection);
                    Connections.Remove(connection);
                }
            }

            foreach (var (name, socket) in node.Outputs)
            {
                foreach (var connection in socket.Connections)
                {
                    connection.Input.Connections.Remove(connection);
                    Connections.Remove(connection);
                }
            }

            Nodes.Remove(node);
        }

        public void AddNodeWithId(Node node)
        {
            IdCounter = Math.Max(node.ID, IdCounter);
            IdCounter++;
            node.NodeManager = this;
            Nodes.Add(node);
        }

        public void DeleteConnection(NodeConnection connection)
        {
            connection.Input.Connections.Remove(connection);
            connection.Output.Connections.Remove(connection);

            Connections.Remove(connection);
        }


        public XElement Serialize()
        {
             return new XElement("material", new XAttribute("name", Name), new XAttribute("output", OutputFileName),
                    Nodes.Select(node =>
                        new XElement("node", new XAttribute("type", node.Name), new XAttribute("id", node.ID),
                              new XElement("position", new XAttribute("X", node.Position.X), new XAttribute("Y", node.Position.Y)),
                              new XElement("data", new XAttribute("value", node.NodeData))
                        )
                    ),
                    Connections.Select(conn =>
                        new XElement("connection",
                            new XElement("input", new XAttribute("id", conn.Input.Parent.ID), new XAttribute("slot", conn.Input.Name)),
                            new XElement("output", new XAttribute("id", conn.Output.Parent.ID), new XAttribute("slot", conn.Output.Name))
                        )
                    )
                );
        }

        public DColor FromString(string str)
        {
            return DColor.White;
        }

        public static TextureView LoadTextureMap(string path)
        {
            var im = SourceImage.LoadImage(path);


            return new TextureView(new TextureMap()
            {
                Data = new List<Image<RgbaVector>>() { im },
                Path = path
            });         
        }

        public static NodeGraph Deserialize(XElement root)
        {
            NodeGraph nodeGraph = new NodeGraph();

            nodeGraph.Name = root.Attribute("name").Value;
            nodeGraph.OutputFileName = root.Attribute("output").Value;

            var nodes = root.Elements("node");
            foreach (var node in nodes)
            {
                var nodeType = node.Attribute("type").Value;
                var nodeId = int.Parse(node.Attribute("id").Value);

                var positionX = node.Element("position").Attribute("X").Value;
                var positionY = node.Element("position").Attribute("Y").Value;

                var nodedata = node.Element("data").Attribute("value").Value;

                Node graphNode = nodeType switch
                {
                    "Texture"       => new TextureNode() { Texture = LoadTextureMap(nodedata) },
                    "Channel Splitter" => new ChannelSplitterNode(),
                    "Channel Mixer"     => new ChannelMixerNode(),
                    "Color"             => new ColorNode(Color32.FromString(nodedata)),
                    "Material"          => new MaterialNode(new NodeEditor.Nodes.Controls.MaterialProperties() { Width = int.Parse(nodedata.Split(":")[0]), Height = int.Parse(nodedata.Split(":")[1]) }),
                    "Environment Map"   => new EnvironmentMapNode(int.Parse(nodedata.Split(":")[0])),
                    "Face List To Cube Map" => new FaceListToCubeMapNode(),
                    "Equirectagular To Cube Map" => new EquirectangularToCubeMapNode(),
                    "Desaturate"          => new DesaturateNode(),
                    "Invert"                => new InvertNode(),
                    "Gaussian Blur"          => new GaussianBlurNode(),
                };

                graphNode.Position = new System.Windows.Point(double.Parse(positionX), double.Parse(positionY));
                graphNode.ID = nodeId;

                nodeGraph.AddNodeWithId(graphNode);
            }

            var connections = root.Elements("connection");
            foreach (var conn in connections)
            {
                var inputId = conn.Element("input").Attribute("id").Value;
                var inputSlot = conn.Element("input").Attribute("slot").Value;
                var outputId = conn.Element("output").Attribute("id").Value;
                var outputSlot = conn.Element("output").Attribute("slot").Value;

                var inputNode = nodeGraph.Nodes.First(x => x.ID.ToString() == inputId);
                var outputNode = nodeGraph.Nodes.First(x => x.ID.ToString() == outputId);

                var connection = new NodeConnection(inputNode.Inputs[inputSlot], outputNode.Outputs[outputSlot]);
                nodeGraph.Connections.Add(connection);
            }

            nodeGraph.Output = nodeGraph.Nodes.First(x => x is MaterialNode || x is EnvironmentMapNode);

            return nodeGraph;
        }

        public FlameEncoder.Data.Material RenderMaterial()
        {
            var maps = new Dictionary<string, TextureMap>();


            int width = (Output as MaterialNode).properties.Width;
            int height = (Output as MaterialNode).properties.Width;

            foreach (var kv in Output.Inputs)
            {
                var name = kv.Key;
                var socket = kv.Value;

                var image = new Image<RgbaVector>(width, height);


                Parallel.For(0, width * height, idx =>
                  {
                      int x = idx / width;
                      int y = idx % height;

                      var finalColor = (Color32)socket.Data(new PixelPosition(x / (float)width, y / (float)height, 0));

                      image[x, y] = new RgbaVector(
                          finalColor.R,
                          finalColor.G,
                          finalColor.B,
                          finalColor.A);

                  });

                maps[name] = new TextureMap()
                {
                    Data = new List<Image<RgbaVector>>() { image },
                };
            }

            var material = new FlameEncoder.Data.Material();

            foreach (var (k,v) in Output.Inputs)
                material.AddMap(k, maps[k]);

            return material;
        }

        public FlameEncoder.Data.EnvMap RenderEnvMap()
        {
            var maps = new Dictionary<string, TextureMap>();


            int width = (Output as EnvironmentMapNode).properties.Width;
            int height = width;


            for (int i = 0; i < 6; i++)
            {
                var socket = Output.Inputs["Specular"];
                var image = new Image<RgbaVector>(width, height);

                Parallel.For(0, width * height, idx =>
                {
                    int x = idx / width;
                    int y = idx % height;

                    var finalColor = (Color32)socket.Data(new PixelPosition(x / (float)width, y / (float)height, i));

                    image[x, y] = new RgbaVector(
                        finalColor.R,
                        finalColor.G,
                        finalColor.B,
                        finalColor.A);

                });

                maps[$"Skymap {FaceNames[i]}"] = new TextureMap()
                {
                    Data = new List<Image<RgbaVector>>() { image },
                };
            }

            for (int i = 0; i < 6; i++)
            {
                maps[$"Specular {FaceNames[i]}"] = new TextureMap()
                {
                    Data = new List<Image<RgbaVector>>() { EnvMapCompiler.ResizeImage(maps[$"Skymap {FaceNames[i]}"].Data[0], 256, 256) },
                };
            }

            for (int i = 0; i < 6; i++)
            {
                var socket = Output.Inputs["Irradiance"];
                var image = new Image<RgbaVector>(256, 256);

                Parallel.For(0, 256 * 256, idx =>
                {
                    int x = idx / 256;
                    int y = idx % 256;

                    var finalColor = (Color32)socket.Data(new PixelPosition(x / (float)256, y / (float)256, i));

                    image[x, y] = new RgbaVector(
                        finalColor.R,
                        finalColor.G,
                        finalColor.B,
                        finalColor.A);

                });

                maps[$"Irradiance {FaceNames[i]}"] = new TextureMap()
                {
                    Data = new List<Image<RgbaVector>>() { image },
                };
            }

            var envMap = new FlameEncoder.Data.EnvMap();

            foreach (var (name, map) in maps)
            {
                envMap.AddMap(name, map);
            }

            return envMap;
        }

    }

}
