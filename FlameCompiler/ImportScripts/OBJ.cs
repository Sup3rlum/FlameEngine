using System;
using System.IO;
using System.IO.Compression;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;
using System.Security.Cryptography;

using FlameCompiler.Data;
using FlameCompiler.Math;


namespace FlameCompiler.ImportScripts
{
    public class OBJ
    {
        public static List<KeyValuePair<DataBuffer, string>> DecodeData(string inputFilePath)
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

            List<KeyValuePair<DataBuffer, string>> meshBuffers = new List<KeyValuePair<DataBuffer, string>>();

            List<List<StaticModelVertex>> vdata = new List<List<StaticModelVertex>>();




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

            List<string> materialNames = new List<string>();

            foreach (string l in lines)
            {
                if (l[0] == 'o')
                {
                    vdata.Add(new List<StaticModelVertex>());
                }
                if (l[0] == 'f')
                {

                    int currentBufferIndex = vdata.Count - 1;

                    string[] tmp = l.Split(' ');


                    string[] v1s = tmp[1].Split('/');
                    string[] v2s = tmp[2].Split('/');
                    string[] v3s = tmp[3].Split('/');


                    Vertex3 vpos1 = vertexData[int.Parse(v1s[0]) - 1];
                    Vertex3 vpos2 = vertexData[int.Parse(v2s[0]) - 1];
                    Vertex3 vpos3 = vertexData[int.Parse(v3s[0]) - 1];

                    Vertex3 vnorm1 = normalData[int.Parse(v1s[2]) - 1];
                    Vertex3 vnorm2 = normalData[int.Parse(v2s[2]) - 1];
                    Vertex3 vnorm3 = normalData[int.Parse(v3s[2]) - 1];


                    Vertex2 vtexcoord1 = texcoordData[int.Parse(v1s[1]) - 1];
                    Vertex2 vtexcoord2 = texcoordData[int.Parse(v2s[1]) - 1];
                    Vertex2 vtexcoord3 = texcoordData[int.Parse(v3s[1]) - 1];

                    Vertex3 edge1 = vpos2 - vpos1;
                    Vertex3 edge2 = vpos3 - vpos1;

                    Vertex2 texEdge1 = vtexcoord2 - vtexcoord1;
                    Vertex2 texEdge2 = vtexcoord3 - vtexcoord1;


                    float radius = 1.0f / (texEdge1.X * texEdge2.Y - texEdge2.X * texEdge1.Y);


    
                    Vertex3 _pTangent = radius * new Vertex3(
                        (texEdge2.Y * edge1.X - texEdge1.Y * edge2.X),
                        (texEdge2.Y * edge1.Y - texEdge1.Y * edge2.Y),
                        (texEdge2.Y * edge1.Z - texEdge1.Y * edge2.Z)
                        );

                    Vertex3 _pBitangent = radius * new Vertex3(
                        (-texEdge2.X * edge1.X + texEdge1.X * edge2.X),
                        (-texEdge2.X * edge1.Y + texEdge1.X * edge2.Y),
                        (-texEdge2.X * edge1.Z + texEdge1.X * edge2.Z)
                        );


                    Vertex3 vtan1 = Vertex3.Normalize(_pTangent -  Vertex3.Dot(vnorm1, _pTangent) * vnorm1);
                    Vertex3 vtan2 = Vertex3.Normalize(_pTangent -  Vertex3.Dot(vnorm2, _pTangent) * vnorm2);
                    Vertex3 vtan3 = Vertex3.Normalize(_pTangent -  Vertex3.Dot(vnorm3, _pTangent) * vnorm3);


                    vdata[currentBufferIndex].Add(new StaticModelVertex(
                        vpos1,
                        vnorm1,
                        vtan1,
                        _pBitangent,
                        vtexcoord1
                        ));


                    vdata[currentBufferIndex].Add(new StaticModelVertex(
                        vpos2,
                        vnorm2,
                        vtan2,
                        _pBitangent,
                        vtexcoord2
                        ));


                    vdata[currentBufferIndex].Add(new StaticModelVertex(
                        vpos3,
                        vnorm3,
                        vtan3,
                        _pBitangent,
                        vtexcoord3
                        ));
                }
                if (l[0] == 'u')
                {
                    materialNames.Add(l.Split(' ')[1]);
                }
            }

            for (int v = 0; v < vdata.Count; v++)
            {

                List<StaticModelVertex> indexedVData = new List<StaticModelVertex>();
                List<uint> indexData = new List<uint>();

                for (int i = 0; i < vdata[v].Count; i++)
                {
                    int index = indexedVData.FindIndex(a => a == vdata[v][i]);

                    if (index >= 0)
                    {
                        indexData.Add((uint)index);
                    }
                    else
                    {
                        indexedVData.Add(vdata[v][i]);

                        indexData.Add((uint)indexedVData.Count - 1);
                    }
                }

                DataBuffer dataBuffer = new DataBuffer();

                dataBuffer.Data = indexedVData.ToArray();
                dataBuffer.IndexData = indexData.ToArray();

                meshBuffers.Add(new KeyValuePair<DataBuffer,string>(dataBuffer, materialNames[v]));
            }

            return meshBuffers;
        }
    }
}
