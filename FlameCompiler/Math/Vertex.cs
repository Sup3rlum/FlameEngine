﻿using System;
using System.Collections.Generic;
using System.Text;

namespace FlameCompiler.Math
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

        public static Vertex2 operator -(Vertex2 a, Vertex2 b)
        {
            return new Vertex2(a.X - b.X, a.Y - b.Y);
        }

        public static Vertex2 operator *(float a, Vertex2 b)
        {
            return new Vertex2(a * b.X, a * b.Y);
        }

        public static float Dot(Vertex2 a, Vertex2 b)
        {
            return a.X * b.X + a.Y * b.Y;
        }

        public static Vertex2 Normalize(Vertex2 a)
        {
            return (1.0f / MathF.Sqrt(Dot(a, a))) * a;
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
            return new Vertex3(a.X + b.X, a.Y + b.Y, a.Z + b.Z);
        }
        public static Vertex3 operator -(Vertex3 a, Vertex3 b)
        {
            return new Vertex3(a.X - b.X, a.Y - b.Y, a.Z - b.Z);
        }
        public static Vertex3 operator*(float a, Vertex3 b)
        {
            return new Vertex3(a * b.X, a * b.Y, a * b.Z);
        }
        public static Vertex3 operator *(Vertex3 b, float a)
        {
            return new Vertex3(a * b.X, a * b.Y, a * b.Z);
        }


        public static float Dot(Vertex3 a, Vertex3 b)
        {
            return a.X * b.X + a.Y * b.Y + a.Z * b.Z;
        }

        public static Vertex3 Normalize(Vertex3 a)
        {
            return (1.0f / MathF.Sqrt(Dot(a, a))) * a;
        }

        public static Vertex3 Cross(Vertex3 a, Vertex3 b)
        {
            return new Vertex3(a.Y * b.Z - a.Z * b.Y,
                a.Z * b.X - a.X * b.Z,
                a.X * b.Y - a.Y * b.X);
        }


        public byte[] GetBytes()
        {
            List<byte> r = new List<byte>();

            r.AddRange(BitConverter.GetBytes(X));
            r.AddRange(BitConverter.GetBytes(Y));
            r.AddRange(BitConverter.GetBytes(Z));

            return r.ToArray();
        }

        public void Deconstruct(out float x, out float y, out float z)
        {
            x = this.X;
            y = this.Y;
            z = this.Z;
        }
    }
}
