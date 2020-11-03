using System;
using System.Collections.Generic;
using System.Text;

namespace ContentCompiler.Data
{
    public class Mesh
    {
        public DataBuffer Buffer;
        public float[] Transform;
        public string MaterialName;
        public string VertShaderSource;
        public string FragShaderSource;

        public Mesh(DataBuffer _d, float[] _transform)
        {
            Buffer = _d;
            Transform = _transform;
        }
    }
}
