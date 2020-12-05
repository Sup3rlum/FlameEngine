using System;
using System.Collections.Generic;
using System.Text;


using FlameCompiler.Math;

namespace FlameCompiler.Data
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

        public static bool operator==(VertexPositionNormalTexture l, VertexPositionNormalTexture r)
        {
            return l.Position.X == r.Position.X &&
                   l.Position.Y == r.Position.Y &&
                   l.Position.Z == r.Position.Z &&

                   l.Normal.X == r.Normal.X &&
                   l.Normal.Y == r.Normal.Y &&
                   l.Normal.Z == r.Normal.Z &&

                   l.TexCoord.X == r.TexCoord.X &&
                   l.TexCoord.Y == r.TexCoord.Y;

        }
        public static bool operator!=(VertexPositionNormalTexture l, VertexPositionNormalTexture r)
        {
            return !(l == r);
        }
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
