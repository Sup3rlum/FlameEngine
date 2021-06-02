using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;
using System.Xml.Linq;

namespace ShaderLibraryLinker
{
    public class BuildScript
    {

        public static LinkerTask Load(string inputFile)
        {
            XDocument xd = XDocument.Load(inputFile);
            ShaderLibrary library = new ShaderLibrary();
            string outputName;

            var xdRoot = xd.Root;

            if (xdRoot.Name != "buildscript" && xdRoot.Attribute("type").Value != "ShaderLibrary")
            {
                throw new Exception();
            }

            var meta = xdRoot.Element("meta");

            string typeName = meta.Element("binary").Attribute("type").Value;

            library.Type = typeName.ToString() switch
            {
                "SPIRV" => LibraryType.SPIRV,
                "DXIL" => LibraryType.DXIL,
                _ => throw new ArgumentException()
            };

            outputName = meta.Element("output").Attribute("path").Value;


            var xdModuleList = xdRoot.Elements("module");


            foreach (var v in xdModuleList)
            {

                ShaderLibraryModule libModule = new ShaderLibraryModule();

                libModule.Name = v.Attribute("name").Value;
                var xdShaders = v.Elements("shader");


                foreach(var s in xdShaders)
                {
                    ShaderLibraryModulePart shaderPart = new ShaderLibraryModulePart();
                    string filePath = s.Attribute("path").Value;

                    shaderPart.partBinary = File.ReadAllBytes(filePath);
                    shaderPart.type = s.Attribute("type").Value.ToString() switch
                    {
                        "Vertex" => ShaderType.Vertex,
                        "Pixel" => ShaderType.Pixel,
                        "Geometry" => ShaderType.Geometry,
                        "Hull" => ShaderType.Hull,
                        "Domain" => ShaderType.Domain,
                        "Compute" => ShaderType.Compute,
                        _ => throw new Exception()
                    };

                    libModule.Shaders.Add(shaderPart.type, shaderPart);
                }

                library.Modules.Add(libModule);
            }


            return new LinkerTask() { outputFileName = outputName, library = library };
        }
    }
}
