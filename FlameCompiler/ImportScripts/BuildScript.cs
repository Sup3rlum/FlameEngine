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
        public Model model;
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
                if (cElement.Attribute("type").Value == "model")
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

            Model model = new Model();

            var meshElements = element.Elements("mesh");
            string outputPath = element.Element("output").Attribute("path").Value;

            foreach (var mElement in meshElements)
            {

                string materialPath = mElement.Element("material").Attribute("path").Value;
                string transformData = mElement.Element("transform").Attribute("data").Value;
                string inputMeshesPath = mElement.Element("meshfile").Attribute("path").Value;


                var transform = transformData.Split(" ").Select(a => float.Parse(a)).ToArray();

                var meshes = OBJ.DecodeData(inputMeshesPath);

                foreach (var mesh in meshes)
                {
                    model.meshCollection.Add(new ModelMesh(mesh.Key, transform, mesh.Value));
                }

                model.worldTransform = transform;
            }

  

            ModelObjects.Add(new ModelTask() { model = model, outputFileName = outputPath });

        }
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

                try
                {
                    im  = Image.Load<Rgba32>(path);
                }
                catch (Exception x)
                {
                    Console.WriteLine(x.Message);
                    continue;
                }

                material.mapDictionary.Add(name, im);

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
