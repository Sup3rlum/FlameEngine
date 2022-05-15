using FlameCompiler.Data;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.IO;
using System.Text;
using System.Xml.Linq;
using System.Drawing;

using FlameCompiler.Compilers;
using System.Linq;

using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Formats;
using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp.Processing;


namespace FlameCompiler.ImportScripts
{

    public class BuildScriptTask
    {
        public string Name { get; set; }
        public string OutputFileName { get; set; }

        public ITextureMapCollection Source { get; set; }
        
        public void ExecuteTask()
        {
            var comp = Source.GetCompiler();
            comp.StartTask(this);
        }

        public string IconName { get
        {
                if (Source.TypeName == "envmap")
                {
                    return "/sphere.png";
                }
                else if (Source.TypeName == "material")
                {
                    return "/tex4.png";
                }
                else
                {
                    return "";
                }
            }
        }
    }

    public class BuildScript
    {

        public string FilePath { get; set; }
        public string ScriptDirectory { get; set; }

        public ObservableCollection<BuildScriptTask> Tasks { get; set; }

        public BuildScript(string scriptPath)
        {
            Tasks = new ObservableCollection<BuildScriptTask>();
            FilePath = scriptPath;
            
            ScriptDirectory = Path.GetDirectoryName(scriptPath);

            Load();
        }
        public bool Load()
        {
            XDocument doc = XDocument.Load(FilePath);

            var currentScript = doc.Element("buildscript");
            var contentElements = currentScript.Elements("content").ToList();

            foreach (var cElement in contentElements)
            {
                LoadContentScript(cElement);
            }
            return true;
        }

        private void LoadContentScript(XElement element)
        {      
            var dataType = element.Attribute("type").Value;

            ITextureMapCollection data = dataType switch
            {
                "material" => new Material(),
                "envmap" => new EnvMap(),
                _ => null
            };

            if (data == null) return;

            string dataName = element.Attribute("name").Value;
            var mapElements = element.Elements("map");
            string outputPath = element.Element("output").Attribute("path").Value;

            if (!Path.IsPathRooted(outputPath))
            {
                outputPath = Path.Combine(ScriptDirectory, outputPath);
            }

            foreach (var map in mapElements)
            {
                string name = map.Attribute("name").Value;
                string path = map.Attribute("path").Value;

                if (!Path.IsPathRooted(path))
                {
                    path = Path.Combine(ScriptDirectory, path);
                }

                int mipLevels = 1;          
                if (map.Attribute("mips") != null)
                    mipLevels = int.Parse(map.Attribute("mips").Value);

                List<Image<Rgba64>> imgData = new List<Image<Rgba64>>();

                var importedImage = Image.Load<Rgba64>(path);
                imgData.Add(importedImage);

                for (int i = 1; i < mipLevels; i++)
                {
                    string directory = Path.GetDirectoryName(path);
                    string flname = Path.GetFileNameWithoutExtension(path) + $"_{i}" + Path.GetExtension(path);

                    imgData.Add(Image.Load(Path.Combine(directory, flname)).CloneAs<Rgba64>());
                }

                
                data.AddMap(name, new TextureMap() 
                { 
                    Name = name,
                    Path = path,
                    Data = imgData 
                });
            }

            Tasks.Add(new BuildScriptTask() 
            { 
                Name = dataName,
                OutputFileName = outputPath,
                Source = data
            });
        }

        public void Execute()
        {
            foreach (var task in Tasks)
            {
                task.ExecuteTask();
            }
        }

        public void SaveAs(string filepath)
        {
            XDocument document = new XDocument(
                new XElement("buildscript",
                    Tasks.Select(task =>   
                        new XElement("content", new XAttribute("type", task.Source.TypeName), new XAttribute("name", task.Name),
                            task.Source.Values.Select(map =>
                                new XElement("map", new XAttribute("name", map.Name), new XAttribute("path", map.Path))
                            ),
                            new XElement("output", new XAttribute("path", task.OutputFileName))
                        )
                    )
                )
            );

            document.Save(filepath);
        }
    }
}
