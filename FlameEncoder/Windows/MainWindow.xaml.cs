using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;
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
using System.Diagnostics;
using System.Xml.Linq;
using System.ComponentModel;
using System.Windows.Forms.Integration;

using Microsoft.Win32;

using FlameEncoderCLR;

using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Formats;
using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp.Processing;

using FlameEncoder.NodeEditor;
using FlameEncoder.NodeEditor.Nodes;

using FlameEncoder.ImportScripts;
using FlameEncoder.Compilers;
using FlameEncoder.Data;


using VPoint = System.Windows.Point;

namespace FlameEncoder
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 

    public class TextureView
    {
        public string Name { get; set; }
        public bool IsActive { get; set; }

        public TextureMap _texMap;
        public TextureMap TextureMap 
        { 
            get
            {
                return _texMap;
            }
            set
            {
                _texMap = value;

                var bitmapImage = new BitmapImage();
                using (var ms = new MemoryStream())
                {
                    TextureMap.Data[0].SaveAsBmp(ms);
                    ms.Seek(0, SeekOrigin.Begin);

                    bitmapImage.BeginInit();
                    bitmapImage.CacheOption = BitmapCacheOption.OnLoad;
                    bitmapImage.StreamSource = ms;
                    bitmapImage.EndInit();
                }

                _image = bitmapImage;
            }
        }

        public BitmapImage _image;
        public BitmapImage Image {
            get
            {
                if (TextureMap == null)
                    return null;

                return _image;
            } }

        public TextureView(TextureMap map)
        {
            TextureMap = map;
        }

    }



    public partial class MainWindow : Window
    {

        public BuildScript OpenProject { get; set; }
        public TextureViewer editorApp;

        public NodeGraph displNodeGraph { get; set; }

        public MainWindow()
        {
            ElementHost.EnableModelessKeyboardInterop(this);


            InitializeComponent();

            texturePreviewPanel.Visibility = Visibility.Hidden;
            nodeGraphPanel.Visibility = Visibility.Hidden;
            StartPage.Visibility = Visibility.Visible;
            MainProgressBar.Visibility = Visibility.Hidden;

        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            editorApp = new TextureViewer(enginePanel.Handle, enginePanel.Width, enginePanel.Height);

            while (editorApp.IsContextActive())
            {
                editorApp.Frame();
                System.Windows.Forms.Application.DoEvents();
            }
        }
        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            editorApp.PollClose();
        }


        public void LoadProject()
        {
            OpenFileDialog openFileDialog = new OpenFileDialog();
            openFileDialog.Filter = "Flame BuildScript files (*.buildscript)|*.buildscript";

            if (openFileDialog.ShowDialog() == true)
            {
                OpenProject = new BuildScript(openFileDialog.FileName);
            }

            Dispatcher.Invoke(() => projectTreeView.ItemsSource = new List<BuildScript>() { OpenProject });
            Dispatcher.Invoke(() => nodeGraphCanvas.DataContext = displNodeGraph);
        }
        public void SaveProject()
        {
            if (OpenProject != null)
            {
                OpenProject.Save(OpenProject.ScriptPath);
            }
        }


        /* ----- File Menu ------------- */

        private void FileMenu_New(object sender, RoutedEventArgs e)
        {
            LoadProject();
        }
        private void FileMenu_Open(object sender, RoutedEventArgs e)
        {
            LoadProject();
        }
        private void FileMenu_Save(object sender, RoutedEventArgs e)
        {
            SaveProject();
        }

        private void TextBlock_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            LoadProject();
        }

        /* -------- TreeView ---------*/


        private void TreeView_SelectedItemChanged(object sender, RoutedPropertyChangedEventArgs<object> e)
        {
            if (e.NewValue.GetType() == typeof(NodeGraph))
            {
                displNodeGraph = e.NewValue as NodeGraph;
                nodeGraphCanvas.DataContext = e.NewValue as NodeGraph;

                nodeGraphPanel.Visibility = Visibility.Visible;
                texturePreviewPanel.Visibility = Visibility.Hidden;
                StartPage.Visibility = Visibility.Hidden;

            }
        }

        private void TreeView_AddGraph(object sender, RoutedEventArgs e)
        {


            NewTask newTaskWin = new NewTask();
            newTaskWin.ShowDialog();

            if (newTaskWin.Graph != null)
            {
                OpenProject.MaterialGraphs.Add(newTaskWin.Graph);
            }
        }

        private void TreeView_CompileMaterialGraph(object sender, RoutedEventArgs e)
        {
            var menuitem = sender as MenuItem;
            var contextmenu = menuitem.Parent as ContextMenu;
            var graph = contextmenu.Tag as NodeGraph;

            new CompilerTask()
            {
                TextureMaps = graph.RenderMaterial(),
                OutputFileName = graph.OutputFileName
            }.Execute();    
        }


        /* --------------  Graph Nodes  ----------------   */

        private void Graph_AddColorNode(object sender, RoutedEventArgs e)
        {
            var node = displNodeGraph.CreateNode<ColorNode>();
            node.Position = Mouse.GetPosition(mainCanvas);
        }

        private void Graph_AddTextureNode(object sender, RoutedEventArgs e)
        {
            var node = displNodeGraph.CreateNode<TextureNode>();
            node.Position = Mouse.GetPosition(mainCanvas);
        }

        private void Graph_AddChannelSplitterNode(object sender, RoutedEventArgs e)
        {
            var node = displNodeGraph.CreateNode<ChannelSplitterNode>();
            node.Position = Mouse.GetPosition(mainCanvas);
        }

        private void Graph_AddChannelMixerNode(object sender, RoutedEventArgs e)
        {
            var node = displNodeGraph.CreateNode<ChannelMixerNode>();
            node.Position = Mouse.GetPosition(mainCanvas);
        }

        private void Graph_AddDesaturateNode(object sender, RoutedEventArgs e)
        {
            var pos = Mouse.GetPosition(mainCanvas);

            var node = displNodeGraph.CreateNode<DesaturateNode>();
            node.Position = Mouse.GetPosition(mainCanvas);
        }

        private void Graph_AddInvertNode(object sender, RoutedEventArgs e)
        {
            var pos = Mouse.GetPosition(mainCanvas);

            var node = displNodeGraph.CreateNode<InvertNode>();
            node.Position = Mouse.GetPosition(mainCanvas);
        }
        private void Graph_AddGaussianBlurNode(object sender, RoutedEventArgs e)
        {
            var pos = Mouse.GetPosition(mainCanvas);

            var node = displNodeGraph.CreateNode<GaussianBlurNode>();
            node.Position = Mouse.GetPosition(mainCanvas);
        }

        private void Graph_AddMaterialNode(object sender, RoutedEventArgs e)
        {
            if (displNodeGraph.Output != null)
            {
                displNodeGraph.DeleteNode(displNodeGraph.Output);
                displNodeGraph.Output = null;
            }
            var node = displNodeGraph.CreateNode<MaterialNode>();
            node.Position = Mouse.GetPosition(mainCanvas);

            displNodeGraph.Output = node;
        }

        /* -------- Toolbar ------------- */

        private void CompileAll_DoWork(object sender, DoWorkEventArgs e)
        {
            var worker = sender as BackgroundWorker;

            double counter = 0;
            OpenProject.MaterialGraphs.ToList().ForEach(graph =>
            {
                new CompilerTask()
                {
                    TextureMaps = graph.RenderMaterial(),
                    OutputFileName = graph.OutputFileName
                }.Execute();

                counter++;
                worker.ReportProgress((int)(counter * 100.0 / OpenProject.MaterialGraphs.Count));
            });
        }

        private async void Toolbar_CompileAll(object sender, RoutedEventArgs e)
        {
            SaveProject();

            MainProgressBar.Visibility = Visibility.Visible;

            var worker = new BackgroundWorker();
            worker.WorkerReportsProgress = true;
            worker.DoWork += new DoWorkEventHandler(CompileAll_DoWork);
            worker.ProgressChanged += (object? sender, ProgressChangedEventArgs e) => { MainProgressBar.Value = e.ProgressPercentage;  };
            worker.RunWorkerCompleted += (object? sender, RunWorkerCompletedEventArgs e) => 
            { 
                MainProgressBar.Value = 0;
                MainProgressBar.Visibility = Visibility.Hidden;
            };
            worker.RunWorkerAsync();
        }

        private void Toolbar_Render(object sender, RoutedEventArgs e)
        {
            var result = displNodeGraph.RenderMaterial();
            SetDisplayMaterial(result);
        }



        /* -------------- Node Editor ------------------*/


        bool isDraggin = false;
        bool isCreatingLink = false;
        Vector offset = new Vector();
        Node movingNode;
        NodeConnection connection;
        System.Windows.Shapes.Rectangle startingRect;


        private void DockPanel_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            isDraggin = true;

            movingNode = (sender as DockPanel).Tag as Node;
            var click = Mouse.GetPosition(mainCanvas);
            offset = click - movingNode.Position;
            
            foreach (var node in displNodeGraph.Nodes)
                node.IsSelected = 0;

            movingNode.IsSelected = 1;

            movingNode.Control.Focus();
        }

        private void DockPanel_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            isDraggin = false;
        }

        private void DockPanel_MouseMove(object sender, MouseEventArgs e)
        {
            var point = Mouse.GetPosition(mainCanvas);

            if (isDraggin && movingNode != null)
            {
                movingNode.Position = point - offset;
            }

            if (isCreatingLink && connection != null)
            {

            }

        }


        private void Image_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            var im = sender as System.Windows.Controls.Image;
            var node = im.Tag as Node;
        
           // node.UpdateData();
        }



        private void mainCanvas_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            //isCreatingLink = false;
        }

        public static IEnumerable<T> FindVisualChilds<T>(DependencyObject depObj) where T : DependencyObject
        {
            if (depObj == null) yield return (T)Enumerable.Empty<T>();
            for (int i = 0; i < VisualTreeHelper.GetChildrenCount(depObj); i++)
            {
                DependencyObject ithChild = VisualTreeHelper.GetChild(depObj, i);
                if (ithChild == null) continue;
                if (ithChild is T t) yield return t;
                foreach (T childOfChild in FindVisualChilds<T>(ithChild)) yield return childOfChild;
            }
        }

        private void DockPanel_Loaded(object sender, RoutedEventArgs e)
        {
            var dock = sender as DockPanel;
            var node = dock.Tag as Node;

            var border = (dock.Children[0] as StackPanel).Children[1] as Border;
            border.Child = node.Control;
           

            foreach (var rect in FindVisualChilds<System.Windows.Shapes.Rectangle>(dock))
            {
                if (rect.Tag is OutputNodeSocket)
                {
                    var sock = rect.Tag as OutputNodeSocket;
                    var relativeLocation = rect.TranslatePoint(new VPoint(0, 0), dock);

                    sock.Position = new ((int)relativeLocation.X, (int)relativeLocation.Y);

                    foreach (var conn in sock.Connections)
                    {
                        conn.NotifyPropertyChanged("SvgString");
                    }
                }
                if (rect.Tag is InputNodeSocket)
                {
                    var sock = rect.Tag as InputNodeSocket;
                    var relativeLocation = rect.TranslatePoint(new VPoint(0, 0), dock);

                    sock.Position = new((int)relativeLocation.X, (int)relativeLocation.Y);

                    foreach (var conn in sock.Connections)
                    {
                        conn.NotifyPropertyChanged("SvgString");
                    }
                }
            }
        }

        private void DockPanel_Unloaded(object sender, RoutedEventArgs e)
        {
            var dock = sender as DockPanel;
            var node = dock.Tag as Node;

            var border = (dock.Children[0] as StackPanel).Children[1] as Border;
            border.Child = null;
        }

        private void Rectangle_MouseLeftButtonUp(object sender, MouseButtonEventArgs e)
        {
            var rect = sender as System.Windows.Shapes.Rectangle;
            var socket = rect.Tag as NodeSocket;

            if (isCreatingLink)
            {
                if (socket is InputNodeSocket)
                    connection.Input = (InputNodeSocket)socket;

                else if (socket is OutputNodeSocket)
                    connection.Output = (OutputNodeSocket)socket;

                connection.Input.Connections.Add(connection);
                connection.Output.Connections.Add(connection);

                displNodeGraph.Connections.Add(connection);

                startingRect.Fill = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Colors.White);

                isCreatingLink = false;
            }
            else
            {
                connection = new NodeConnection();
                startingRect = rect;

                startingRect.Fill = new System.Windows.Media.SolidColorBrush(System.Windows.Media.Colors.LightBlue); 

                if (socket is InputNodeSocket)
                    connection.Input = (InputNodeSocket)socket;

                else if (socket is OutputNodeSocket)
                    connection.Output = (OutputNodeSocket)socket;

                isCreatingLink = true;
            }
        }



        private void Rectangle_MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            
        }

        public void SetFlameMaterial(Material result)
        {
            TextureDesc[] descs = new TextureDesc[4];

            var diffuse = result.Maps["Diffuse"].Data[0];
            var data = MaterialCompiler.ImageToByteArray32(diffuse);

            descs[0] = new TextureDesc();
            descs[0].Data = data;
            descs[0].width = diffuse.Width;
            descs[0].height = diffuse.Height;

            var normal = result.Maps["Normal"].Data[0];
            data = MaterialCompiler.ImageToByteArray32(normal);

            descs[1] = new TextureDesc();
            descs[1].Data = data;
            descs[1].width = normal.Width;
            descs[1].height = normal.Height;


            var height = result.Maps["Height"].Data[0];
            var rough = result.Maps["Roughness"].Data[0];
            var metal = result.Maps["Metallic"].Data[0];
            var ao = result.Maps["AO"].Data[0];

            int max_width =
                Math.Max(
                    Math.Max(
                        height.Width,
                        rough.Width
                        ),
                    Math.Max(
                        metal.Width,
                        ao.Width));
            int max_height =
                Math.Max(
                    Math.Max(
                        height.Height,
                        rough.Height
                        ),
                    Math.Max(
                        metal.Height,
                        ao.Height));

            height = MaterialCompiler.ResizeImage(height, max_width, max_height);
            rough = MaterialCompiler.ResizeImage(rough, max_width, max_height);
            metal = MaterialCompiler.ResizeImage(metal, max_width, max_height);
            ao = MaterialCompiler.ResizeImage(ao, max_width, max_height);


            data = MaterialCompiler.FromChannels(
                MaterialCompiler.ImageToByteArray32(height),
                MaterialCompiler.ImageToByteArray32(rough),
                MaterialCompiler.ImageToByteArray32(metal),
                MaterialCompiler.ImageToByteArray32(ao)
                );

            descs[2] = new TextureDesc();
            descs[2].width = max_width;
            descs[2].height = max_height;
            descs[2].Data = data;

            var emissive = result.Maps["Emissive"].Data[0];
            data = MaterialCompiler.ImageToByteArray32(emissive);

            descs[3] = new TextureDesc();
            descs[3].Data = data;
            descs[3].width = emissive.Width;
            descs[3].height = emissive.Height;

            editorApp.SetMaterial(descs);
        }

        public void SetDisplayMaterial(Material result)
        {
            var display = result.Maps.Select(x => new TextureView(x.Value) { Name = x.Key });
            matListBox.ItemsSource = display;

            SetFlameMaterial(result);
        }


        private void DeleteNode_Click(object sender, RoutedEventArgs e)
        {
            var button = sender as Button;
            var node = button.Tag as Node;

            displNodeGraph.DeleteNode(node);
        }

        private void Rectangle_MouseRightButtonDown(object sender, MouseButtonEventArgs e)
        {
            var rect = sender as System.Windows.Shapes.Rectangle;
            var socket = rect.Tag as NodeSocket;


            var connections = new List<NodeConnection>();

            foreach(var conn in socket.Connections)
            {
                connections.Add(conn);
            }

            foreach (var conn in connections)
            {
                displNodeGraph.DeleteConnection(conn);
            }
        }
    }
}
