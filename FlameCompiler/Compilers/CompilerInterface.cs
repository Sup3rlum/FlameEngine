using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using FlameCompiler.ImportScripts;

namespace FlameCompiler.Compilers
{
    public interface ITaskCompiler
    {
        public void StartTask(BuildScriptTask task);
    }
}
