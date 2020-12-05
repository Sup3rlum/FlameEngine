using System;
using System.Collections.Generic;
using System.Text;

namespace FlameCompiler.Data
{
    public class ModelMesh
    {
        public DataBuffer Buffer;
        public float[] localTransform;
        public string MaterialName;


        public ModelMesh(DataBuffer _d, float[] _transform, string materialName)
        {
            Buffer = _d;
            localTransform = _transform;
            MaterialName = materialName;
        }
    }
}
