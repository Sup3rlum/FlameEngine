using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Xml.Linq;
using System.IO;

using FlameEncoder.NodeEditor;

namespace FlameEncoder.ImportScripts
{
    public class BuildScript
    {
        public ObservableCollection<NodeGraph> MaterialGraphs { get; set; }

        public string ScriptDirectory => Path.GetDirectoryName(ScriptPath);
        public string ScriptPath { get; set; }

        public BuildScript(string path)
        {
            ScriptPath = Path.GetFullPath(path);

            MaterialGraphs = new ObservableCollection<NodeGraph>();
            XDocument doc = XDocument.Load(path);

            foreach (var graphElem in doc.Element("buildscript").Elements("material"))
            {
                try
                {
                    var graph = NodeGraph.Deserialize(graphElem);

                    if (!Path.IsPathRooted(graph.OutputFileName))
                        graph.OutputFileName = Path.Combine(ScriptDirectory, graph.OutputFileName);

                    MaterialGraphs.Add(graph);
                }
                catch (Exception ex)
                {
                    Console.WriteLine(ex.Message);
                }
            }
        }

        public void Save(string path)
        {
            XDocument doc = new XDocument(new XElement("buildscript",
                MaterialGraphs.Select(graph => graph.Serialize()
                )));

            doc.Save(path);
        }
    }
}
