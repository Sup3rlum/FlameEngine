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

using ContentCompiler.ImportScripts;

namespace ContentCompiler.Compilers
{
    public struct CompilationArguments
    {
        public string OutputFilepath;
        public bool Static;
    }

    public class ModelFileCompiler
    {

        CompilationArguments _args;

        readonly byte[] Version = new byte[] { 1, 0, 0, 0 };
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


            EncodeMeshTable(task.model, ref memory);
            foreach (var mesh in task.model.meshCollection)
            {
                EncodeMesh(mesh, ref memory);
            }


            OmitBinary(ref memory, task.outputFileName);
        }
        private void EncodeMeshTable(Model model, ref MemoryStream memory)
        {
            memory.Write(BitConverter.GetBytes(model.meshCollection.Count));
        }

        private void EncodeMesh(ModelMesh mesh, ref MemoryStream memory)
        {

            for (int i = 0; i < 16; i++)
            {
                memory.Write(BitConverter.GetBytes(mesh.localTransform[i])); // local Matrix
            }

            memory.Write(BitConverter.GetBytes(mesh.MaterialName.Length));  // materialPathLength
            memory.Write(Encoding.ASCII.GetBytes(mesh.MaterialName));       // materialPath


            memory.Write(BitConverter.GetBytes((ulong)(mesh.Buffer.Data.Length * VertexPositionNormalTexture.Size * 4)));   // vDataLength
            memory.Write(BitConverter.GetBytes((ulong)mesh.Buffer.IndexData.Length * 4));                                   // iDataLength

            foreach (var v in mesh.Buffer.Data)
            {
                memory.Write(v.Position.GetBytes());

                memory.Write(v.Normal.GetBytes());          //vData
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
            _mem.Write(BitConverter.GetBytes(_args.Static ? 1 : 0));    // Compressed
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
    }
}
