using FlameCompiler.Data;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml.Linq;

using FlameCompiler.Compilers;
using System.Linq;

using SixLabors.ImageSharp;
using SixLabors.ImageSharp.ColorSpaces;
using SixLabors.ImageSharp.Formats;
using SixLabors.ImageSharp.PixelFormats;



namespace FlameCompiler.ImportScripts
{
    public struct ModelTask
    {
        public Mesh mesh;
        public string outputFileName;
    }
    public struct MaterialTask
    {
        public Material material;
        public string outputFileName;
    }


    public class BuildScript
    {

        public string _filePath;

        List<ModelTask> ModelObjects = new List<ModelTask>();
        List<MaterialTask> MaterialObjects = new List<MaterialTask>();


        public BuildScript(string scriptPath)
        {
            _filePath = scriptPath;

        }
        public bool Load()
        {
            XDocument doc = XDocument.Load(_filePath);


            var currentScript = doc.Element("buildscript");

            var contentElements = currentScript.Elements("content").ToList();


            foreach (var cElement in contentElements)
            {
                if (cElement.Attribute("type").Value == "mesh")
                {
                    LoadModelScript(cElement);
                }
                if (cElement.Attribute("type").Value == "material")
                {
                    LoadMaterialScript(cElement);
                }


            }


            return true;
        }


        private void LoadModelScript(XElement element)
        {

            Mesh mesh;

            string inputMeshPath = element.Element("source").Attribute("path").Value;
            string outputPath = element.Element("output").Attribute("path").Value;



            var meshes = OBJ.DecodeData(inputMeshPath);


            mesh = new Mesh(meshes[0].Key);

 
            ModelObjects.Add(new ModelTask() { mesh = mesh, outputFileName = outputPath });

        }



        private MaterialMapType GetMaterialMapType(string name) => name switch
        {
            "Diffuse" => MaterialMapType.Diffuse,
            "Normal" => MaterialMapType.Normal,
            "Height" => MaterialMapType.Height,
            "Roughness" => MaterialMapType.Roughness,
            "Metallic" => MaterialMapType.Metallic,
            "AO" => MaterialMapType.AmbientOcclusion,
            _ => throw new Exception("Material Map type not recognized")
        };

        private void LoadMaterialScript(XElement element)
        {
            Material material = new Material();

            var mapElements = element.Elements("map");
            string outputPath = element.Element("output").Attribute("path").Value;

            foreach (var map in mapElements)
            {
                string name = map.Attribute("name").Value;
                string path = map.Attribute("path").Value;

                Image<Rgba32> im;
                MaterialMapType type;


                try
                {
                    im  = Image.Load<Rgba32>(path);
                    type = GetMaterialMapType(name);
                }
                catch (Exception x)
                {
                    Console.WriteLine(x.Message);
                    continue;
                }

                material.mapArray[(int)type] = im;

            }




            MaterialObjects.Add(new MaterialTask() { material = material, outputFileName = outputPath });

        }

        public void Execute()
        {
            CompilationArguments args = new CompilationArguments();
            ModelFileCompiler mfc = new ModelFileCompiler(args);
            MaterialFileCompiler mtc = new MaterialFileCompiler(args);

            foreach (var modtask in ModelObjects)
            {
                mfc.StartTask(modtask);
            }
            foreach (var mattask in MaterialObjects)
            {
                mtc.StartTask(mattask);
            }
        }
    }
}
