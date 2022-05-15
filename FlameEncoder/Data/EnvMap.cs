using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
using System.Linq;
using System.Runtime.InteropServices;
using System.Drawing;

using FlameEncoder.Compilers;

namespace FlameEncoder.Data
{
    public class EnvMap : TextureMapCollection
    {
        public override ITaskCompiler CreateCompiler()
        {
            return new EnvMapCompiler();
        }
    }
}
