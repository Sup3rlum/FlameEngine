using System;
using System.Collections.Generic;
using System.Text;

namespace AssetEncoderCommandline.Math
{
    public class Vertex2
    {
        public float X, Y;

        public Vertex2(float x, float y)
        {
            X = x;
            Y = y;
        }

        public float this[int i]
        {
            get
            {
                if (i == 0) return X;
                else if (i == 1) return Y;
                else throw new IndexOutOfRangeException();
            }
            set
            {
                if (i == 0) X = value;
                else if (i == 1) Y = value;
                else throw new IndexOutOfRangeException();
            }
        }

        public static Vertex2 operator +(Vertex2 a, Vertex2 b)
        {
            return new Vertex2(a.X + b.X, a.Y + b.Y);
        }

        public byte[] GetBytes()
        {
            List<byte> r = new List<byte>();

            r.AddRange(BitConverter.GetBytes(X));
            r.AddRange(BitConverter.GetBytes(Y));

            return r.ToArray();
        }
    }
    public class Vertex3
    {
        public float X, Y, Z;

        public Vertex3(float x, float y, float z)
        {
            X = x;
            Y = y;
            Z = z;
        }

        public float this[int i]    
        {
            get 
            {
                if (i == 0) return X;
                else if (i == 1) return Y;
                else if (i == 2) return Z;
                else throw new IndexOutOfRangeException();
            }
            set
            {
                if (i == 0) X = value;
                else if (i == 1) Y = value;
                else if (i == 2) Z = value;
                else throw new IndexOutOfRangeException();
            }
        }
        public static Vertex3 operator +(Vertex3 a, Vertex3 b)
        {
            return new Vertex3(a.X + b.X, a.Y + b.Y,a.Z + b.Z);
        }

        public byte[] GetBytes()
        {
            List<byte> r = new List<byte>();

            r.AddRange(BitConverter.GetBytes(X));
            r.AddRange(BitConverter.GetBytes(Y));
            r.AddRange(BitConverter.GetBytes(Z));

            return r.ToArray();
        }
    }
}
