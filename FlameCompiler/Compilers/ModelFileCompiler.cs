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

using FlameCompiler.ImportScripts;

namespace FlameCompiler.Compilers
{
    public struct CompilationArguments
    {
        public string OutputFilepath;
        public bool Static;
    }

    public class ModelFileCompiler
    {

        CompilationArguments _args;

        readonly byte[] Version = new byte[] { 1, 0, 0, 2 };
        readonly byte[] Signature = { (byte)'F', (byte)'L', (byte)'3', (byte)'D' };


        public ModelFileCompiler(CompilationArguments _args)
        {
            this._args = _args;
        }


        public void StartTask(ModelTask task)
        {
            // (new Task(() => StartTaskInternal(mesh))).Start();

            StartTaskInternal(task);

        }

        private void StartTaskInternal(ModelTask task)
        {

            MemoryStream memory = new MemoryStream();


            EncodeMesh(task.mesh, ref memory);


            OmitBinary(ref memory, task.outputFileName);
        }

        private void EncodeMesh(Mesh mesh, ref MemoryStream memory)
        {

            memory.Write(BitConverter.GetBytes((ulong)mesh.Buffer.Data.Length));   // vDataLength
            memory.Write(BitConverter.GetBytes((ulong)mesh.Buffer.vertexDeclaration.VertexSize));   // vertexSize

            memory.Write(BitConverter.GetBytes((ulong)mesh.Buffer.IndexData.Length * 4));                                   // iDataLength
            memory.Write(BitConverter.GetBytes((uint)EFRIIndexType.UINT32));                                   // iDataLength
            memory.Write(BitConverter.GetBytes((uint)mesh.Buffer.vertexDeclaration.VertComponents.Length));                                   // iDataLength

            foreach (var v in mesh.Buffer.vertexDeclaration.VertComponents)
            {
                memory.Write(BitConverter.GetBytes(v.AttribNumber));
                memory.Write(BitConverter.GetBytes(v.Length));
                memory.Write(BitConverter.GetBytes((uint)v.Type));
                memory.Write(BitConverter.GetBytes(v.Normalized ? (uint)1 : (uint)0));
                memory.Write(BitConverter.GetBytes(v.Stride));
                memory.Write(BitConverter.GetBytes(v.Offset));
            }

            foreach (var v in mesh.Buffer.Data)
            {
                memory.Write(v.Position.GetBytes());
                memory.Write(v.Normal.GetBytes());          //vData
                memory.Write(v.Tangent.GetBytes());          //vData
                memory.Write(v.Bitangent.GetBytes());          //vData
                memory.Write(v.TexCoord.GetBytes());
            }

            foreach (var v in mesh.Buffer.IndexData)
            {
                memory.Write(BitConverter.GetBytes(v));     //iData
            }


        }

        private MemoryStream SignBinary(ref MemoryStream memory)
        {

            // --- Checksum

            var data = memory.ToArray();
            var md5 = MD5.Create();
            var checksum = md5.ComputeHash(data);


            MemoryStream _mem = new MemoryStream();


            // -- Header

            _mem.Write(Signature);                                      // Signature    
            _mem.Write(Version);                                        // Version
            //_mem.Write(BitConverter.GetBytes(_args.Static ? 1 : 0));    // Compressed
            _mem.Write(checksum);                                       // Checksum

            _mem.Write(data);                                           // Data

            return _mem;
        }
        private void OmitBinary(ref MemoryStream memory, string outputFile)
        {

            FileStream _fStream = new FileStream(outputFile, FileMode.Create);

            var bin = SignBinary(ref memory);

            bin.WriteTo(_fStream);

            _fStream.Close();
        }

        /*
        public void EncodeBytes<T>(ref MemoryStream memory, T data )
        {
            memory.Write(BitConverter.GetBytes(data));
        }*/

    }
}
