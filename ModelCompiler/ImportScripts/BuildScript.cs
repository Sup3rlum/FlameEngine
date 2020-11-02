using System;
using System.Collections.Generic;
using System.Text;
using System.Xml.Linq;

namespace ContentCompiler.ImportScripts
{
    public class BuildScript
    {
        public BuildScript(string scriptPath)
        {
            XDocument doc = XDocument.Load(scriptPath);

        }
    }
}
