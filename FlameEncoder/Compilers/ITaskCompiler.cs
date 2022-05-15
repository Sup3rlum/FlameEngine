using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using FlameEncoder.NodeEditor;
using FlameEncoder.Data;

namespace FlameEncoder.Compilers
{
    public interface ITaskCompiler
    {
        public void StartTask(CompilerTask task);
    }

    public class CompilerTask
    {
        public TextureMapCollection TextureMaps;
        public string OutputFileName;

        public void Execute()
        {
            var compiler = TextureMaps.CreateCompiler();
            compiler.StartTask(this);
        }
    }
}
