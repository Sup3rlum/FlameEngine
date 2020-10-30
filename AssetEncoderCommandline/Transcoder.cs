using System;
using System.IO;
using System.IO.Compression;
using System.Collections.Generic;
using System.Text;
using AssetEncoderCommandline.Math;
using System.Runtime.CompilerServices;



namespace AssetEncoderCommandline
{

    public class VertexPositionNormalTexture
    {
        public Vertex3 Position;
        public Vertex3 Normal;
        public Vertex2 TexCoord;


        public VertexPositionNormalTexture(Vertex3 p , Vertex3 n, Vertex2 t)
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


    public class Transcoder
    {

        public static byte[] Version = { 1, 0, 0, 0 };

        public static void Convert(string inputFilePath, string outputFilePath)
        {
            var db = Decode(inputFilePath);

            Encode(outputFilePath, db);
        }
        public static void Encode(string outputFilepath, DataBuffer buffer)
        {
            int size = 24 + buffer.Data.Length * VertexPositionNormalTexture.Size * 4 + buffer.IndexData.Length * 4;

            MemoryStream mem = new MemoryStream(size);

            mem.WriteByte((byte)'D');
            mem.WriteByte((byte)'E');
            mem.WriteByte((byte)'M');
            mem.WriteByte((byte)'F');

            mem.Write(Version);

            mem.Write(BitConverter.GetBytes((ulong)(buffer.Data.Length * VertexPositionNormalTexture.Size * 4)));
            mem.Write(BitConverter.GetBytes((ulong)buffer.IndexData.Length * 4));

            Console.WriteLine((ulong)(buffer.Data.Length * VertexPositionNormalTexture.Size * 4));
            Console.WriteLine((ulong)buffer.IndexData.Length * 4);

            foreach (var v in buffer.Data)
            {
                mem.Write(v.Position.GetBytes());
                mem.Write(v.Normal.GetBytes());
                mem.Write(v.TexCoord.GetBytes());
            }

            Console.WriteLine("Mem length is: " + mem.Length);


            foreach (var v in buffer.IndexData)
            {
                mem.Write(BitConverter.GetBytes(v));
            }


            Console.WriteLine("Mem length is: " + mem.Length);




            var c = Compress(mem.ToArray());


            MemoryStream mem2 = new MemoryStream(c.Length);
            mem2.Write(c);


            FileStream fStream = new FileStream(outputFilepath, FileMode.Create);

            mem2.WriteTo(fStream);

        }

        public static DataBuffer Decode(string inputFilePath)
        {
            StreamReader sr = new StreamReader(inputFilePath);

            List<string> lines = new List<string>();

            while (!sr.EndOfStream)
            {
                lines.Add(sr.ReadLine());
            }

            List<Vertex3> vertexData = new List<Vertex3>();
            List<Vertex3> normalData = new List<Vertex3>();
            List<Vertex2> texcoordData = new List<Vertex2>();

            List<VertexPositionNormalTexture> vdata = new List<VertexPositionNormalTexture>();

            foreach (string l in lines)
            {
                if (l[0] == 'v')
                {

                    string[] tmp = l.Split(' ');

                    if (l[1] == ' ')
                    {


                        float x = float.Parse(tmp[1]);
                        float y = float.Parse(tmp[2]);
                        float z = float.Parse(tmp[3]);

                        vertexData.Add(new Vertex3(x, y, z));
                        
                    }
                    if (l[1] == 'n')
                    {


                        float x = float.Parse(tmp[1]);
                        float y = float.Parse(tmp[2]);
                        float z = float.Parse(tmp[3]);

                        normalData.Add(new Vertex3(x, y, z));

                    }
                    if (l[1] == 't')
                    {


                        float x = float.Parse(tmp[1]);
                        float y = float.Parse(tmp[2]);

                        texcoordData.Add(new Vertex2(x, y));

                    }
                }

            }
            foreach (string l in lines)
            {
                if (l[0] == 'f')
                {
                    string[] tmp = l.Split(' ');


                    string[] v1s = tmp[1].Split('/');
                    string[] v2s = tmp[2].Split('/');
                    string[] v3s = tmp[3].Split('/');

                    vdata.Add(new VertexPositionNormalTexture(
                        vertexData[int.Parse(v1s[0]) - 1],
                        normalData[int.Parse(v1s[2]) - 1],
                        texcoordData[int.Parse(v1s[1]) - 1]
                        ));


                    vdata.Add(new VertexPositionNormalTexture(
                        vertexData[int.Parse(v2s[0]) - 1],
                        normalData[int.Parse(v2s[2]) - 1],
                        texcoordData[int.Parse(v2s[1]) - 1]
                        ));


                    vdata.Add(new VertexPositionNormalTexture(
                        vertexData[int.Parse(v3s[0]) - 1],
                        normalData[int.Parse(v3s[2]) - 1],
                        texcoordData[int.Parse(v3s[1]) - 1]
                        ));
                }
            }

            List<VertexPositionNormalTexture> indexedVData = new List<VertexPositionNormalTexture>();
            List<uint> indexData = new List<uint>();

            for (int i = 0; i < vdata.Count; i++)
            {
                int index = indexedVData.FindIndex(a => a == vdata[i]);

                if (index >= 0)
                {
                    indexData.Add((uint)index);
                }
                else
                {
                    indexedVData.Add(vdata[i]);

                    indexData.Add((uint)indexedVData.Count - 1);
                }
            }

            DataBuffer dataBuffer = new DataBuffer();

            dataBuffer.Data = indexedVData.ToArray();
            dataBuffer.IndexData = indexData.ToArray();


            return dataBuffer;

        }
        private static byte[] Compress(byte[] data)
        {
            using (var compressedStream = new MemoryStream())
            using (var zipStream = new GZipStream(compressedStream, CompressionMode.Compress))
            {
                zipStream.Write(data, 0, data.Length);
                return compressedStream.ToArray();
            }
        }
    }
}
