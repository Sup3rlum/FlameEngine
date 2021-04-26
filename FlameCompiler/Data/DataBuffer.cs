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



    public class StaticModelVertex
    {
        public Vertex3 Position;
        public Vertex3 Normal;
        public Vertex3 Tangent;
        public Vertex3 Bitangent;
        public Vertex2 TexCoord;


        public StaticModelVertex(Vertex3 p, Vertex3 n, Vertex3 t, Vertex3 b, Vertex2 tc)
        {
            Position = p;
            Normal = n;
            Tangent = t;
            Bitangent = b;
            TexCoord = tc;
        }

        public static int Size = 14;

        public static bool operator ==(StaticModelVertex l, StaticModelVertex r)
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
        public static bool operator !=(StaticModelVertex l, StaticModelVertex r)
        {
            return !(l == r);
        }
    }


    public enum EFRIVertexDeclerationAttributeType : uint
    {
        Float = 0x1406,
        Int = 0x1404
    }

    enum EFRIIndexType : uint
    {
        UINT8 = 0x1401,
	    UINT16 = 0x1403,
	    UINT32 = 0x1405
    }

    public class VertexDeclerationComponent
    {
        public uint AttribNumber;
        public EFRIVertexDeclerationAttributeType Type;
        public bool Normalized;
        public uint Stride;
        public uint Length;
        public uint Offset;

        public VertexDeclerationComponent(uint a, uint l, EFRIVertexDeclerationAttributeType t, bool n, uint s, uint o)
        {
            AttribNumber = a;
            Type = t;
            Normalized = n;
            Stride = s;
            Length = l;
            Offset = o;
        }

    }

    public class VertexDeclaration
    {
        public int VertexSize;
        public VertexDeclerationComponent[] VertComponents;
    }


    public class DataBuffer
    {
        public VertexDeclaration vertexDeclaration;
        public StaticModelVertex[] Data;
        public uint[] IndexData;

        public DataBuffer()
        {

        }
    }
}
