using System;
using System.Collections.Generic;
using System.Text;


using ContentCompiler.Math;

namespace ContentCompiler.Data
{
    public class VertexPositionNormalTexture
    {
        public Vertex3 Position;
        public Vertex3 Normal;
        public Vertex2 TexCoord;


        public VertexPositionNormalTexture(Vertex3 p, Vertex3 n, Vertex2 t)
        {
            Position = p;
            Normal = n;
            TexCoord = t;
        }

        public static int Size = 8;
    }

    public class DataBuffer
    {
        public VertexPositionNormalTexture[] Data;
        public uint[] IndexData;

        public DataBuffer()
        {

        }
    }
}
