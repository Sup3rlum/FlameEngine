using System;
using System.Collections.Generic;
using System.Text;


using ContentCompiler.Data;


namespace AssetEncoderCommandline
{
    public class Mesh
    {
        public DataBuffer Buffer;
        public int ID;
        public int ParentID;
        public float[] Transform;
        public string MaterialName;

        public Mesh(DataBuffer _d, int _id, float[] _transform)
        {
            Buffer = _d;
            ID = _id;
            Transform = _transform;
        }
    }
}
