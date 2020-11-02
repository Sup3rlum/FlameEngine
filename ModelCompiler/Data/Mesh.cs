using System;
using System.Collections.Generic;
using System.Text;

namespace ContentCompiler.Data
{
    public class Mesh
    {
        public DataBuffer Buffer;
        public int ID;
        public int ParentID;
        public float[] Transform;
        public string MaterialName;
        public string ShaderSource;

        public Mesh(DataBuffer _d, int _id, float[] _transform)
        {
            Buffer = _d;
            ID = _id;
            Transform = _transform;
        }
    }
}
