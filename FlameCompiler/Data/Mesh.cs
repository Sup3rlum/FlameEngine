using System;
using System.Collections.Generic;
using System.Text;

namespace FlameCompiler.Data
{
    public class Mesh
    {
        public DataBuffer Buffer;

        public Mesh(DataBuffer _d)
        {
            Buffer = _d;
        }
    }
}
