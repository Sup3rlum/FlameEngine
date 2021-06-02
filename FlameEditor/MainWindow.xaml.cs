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
using System.Windows.Interop;
using System.Diagnostics;
using System.Windows.Threading;

using FlameEditorCLR;
using FlameEditor.Engine;

namespace FlameEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {

        public ObservableCollection<Scene> scenes = new ObservableCollection<Scene>() { };

        public MainWindow()
        {
            InitializeComponent();

            editorTreeView.ItemsSource = scenes;


        }

        public void InitEngineVis()
        {
            EditorApp editorApp = new EditorApp(enginePanel.Handle, 0, 0, enginePanel.Width, enginePanel.Height);
            editorApp.Load();


            while (editorApp.IsContextActive())
            {
                editorApp.Frame();
                System.Windows.Forms.Application.DoEvents();
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {




        }

        private void Button_Click_1(object sender, RoutedEventArgs e)
        {

            Entity testEntity = new Entity("Entity");

            testEntity.Components.Add(new EntityComponent() { Name = "MeshComponent" });
            testEntity.Components.Add(new EntityComponent() { Name = "TransformComponent" });
            testEntity.Components.Add(new EntityComponent() { Name = "CameraComponent" });

            Scene testScene = new Scene("Hello");
            testScene.EntityCollection.Add(testEntity);

            scenes.Add(testScene);
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            InitEngineVis();
        }
    }
}
