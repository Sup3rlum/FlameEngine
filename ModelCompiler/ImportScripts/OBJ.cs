using System;
using System.IO;
using System.IO.Compression;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;
using System.Security.Cryptography;

using ContentCompiler.Data;
using ContentCompiler.Math;


namespace ContentCompiler.ImportScripts
{
    public class OBJ
    {
        public static DataBuffer DecodeData(string inputFilePath)
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
    }
}
