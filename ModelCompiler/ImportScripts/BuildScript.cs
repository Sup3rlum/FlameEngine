using ContentCompiler.Data;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml.Linq;

using ContentCompiler.Compilers;
using System.Linq;

namespace ContentCompiler.ImportScripts
{
    public class BuildScript
    {

        public string _outputFilePath;
        public string _filePath;
        Mesh _mesh;


        public BuildScript(string scriptPath)
        {
            _filePath = scriptPath;

        }
        public bool Load()
        {
            XDocument doc = XDocument.Load(_filePath);

            var shaderElement = doc.Element("buildscript").Element("shader");

            var vertElement = shaderElement.Element("vert");
            var geomElement = shaderElement.Element("geom");
            var fragElement = shaderElement.Element("frag");

            if (vertElement == null || fragElement == null)
            {
                return false;
            }

            string vPath = vertElement.Attribute("path").Value;
            string fPath = fragElement.Attribute("path").Value;

            var meshElement = doc.Element("buildscript").Element("mesh");

            var transformElement = meshElement.Element("transform");
            var dataElement = meshElement.Element("data");

            string transformData = transformElement.Attribute("data").Value;
            string inputModelPath = dataElement.Attribute("path").Value;

            string materialPath = doc.Element("buildscript").Element("material").Attribute("path").Value;

            var transform = transformData.Split(" ").Select(a => float.Parse(a)).ToArray();

            var data = OBJ.DecodeData(inputModelPath);

            _mesh = new Mesh(data, transform);

            _mesh.Buffer = data;
            _mesh.MaterialName = materialPath;
            _mesh.VertShaderSource = File.ReadAllText(vPath);
            _mesh.FragShaderSource = File.ReadAllText(fPath);

            _outputFilePath = doc.Element("buildscript").Element("output").Attribute("path").Value;

            return true;
        }

        public void Execute()
        {
            if (_mesh == null)
            {
                throw new NotImplementedException();
            }

            CompilationArguments args = new CompilationArguments();
            args.Static = true;
            args.OutputFilepath = _outputFilePath;

            ModelFileCompiler mfc = new ModelFileCompiler(args);

            mfc.StartTask(_mesh);
        }
    }
}
