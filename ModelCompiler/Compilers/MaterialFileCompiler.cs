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
    public class MaterialFileCompiler
    {


        CompilationArguments _args;

        readonly byte[] Version = new byte[] { 1, 0, 0, 0 };
        readonly byte[] Signature = { (byte)'F', (byte)'L', (byte)'M', (byte)'T' };


        public MaterialFileCompiler(CompilationArguments _args)
        {
            this._args = _args;
        }


        public void StartTask(MaterialTask task)
        {
            Console.WriteLine($"Starting task: {task.outputFileName}");

            StartTaskInternal(task);

        }

        private void StartTaskInternal(MaterialTask task)
        {

            MemoryStream memory = new MemoryStream();


            EncodeMaterial(task.material, ref memory);

            OmitBinary(ref memory, task.outputFileName);
        }

        private void EncodeMaterial(Material material, ref MemoryStream memory)
        {
                memory.Write(BitConverter.GetBytes(material.colormapPath.Length)); 
                memory.Write(Encoding.ASCII.GetBytes(material.colormapPath)); 
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
