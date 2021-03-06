﻿using System;
using System.IO;
using System.IO.Compression;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;
using System.Security.Cryptography;
using System.Linq;
using System.Runtime.InteropServices;

using SixLabors.ImageSharp;
using SixLabors.ImageSharp.ColorSpaces;
using SixLabors.ImageSharp.Formats;
using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp.Advanced;
using SixLabors.ImageSharp.Memory;


using FlameCompiler.Data;
using FlameCompiler.Math;

using FlameCompiler.ImportScripts;

namespace FlameCompiler.Compilers
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

            int mapCount = 0;

            for (int i = 0; i < material.mapArray.Length; i++)
            {
                if (material.mapArray[i] != null)
                {
                    mapCount++;
                }
            }


            memory.Write(BitConverter.GetBytes(mapCount));    // Map Count


            for (int i = 0; i < material.mapArray.Length; i++)
            {
                if (material.mapArray[i] != null)
                {

                    memory.Write(BitConverter.GetBytes(i));    // Map Type Enum
                    memory.Write(BitConverter.GetBytes(material.mapArray[i].Width));   // Map Width
                    memory.Write(BitConverter.GetBytes(material.mapArray[i].Height));  // Map Height

                    int pixelformatname = material.mapArray[i].PixelType.BitsPerPixel / 8; // Channels

                    memory.Write(BitConverter.GetBytes(pixelformatname)); // Pixel Channels
                    memory.Write(BitConverter.GetBytes(pixelformatname)); // Pixel Format
                    memory.Write(BitConverter.GetBytes(0)); // dummy



                    var _IMemoryGroup = material.mapArray[i].GetPixelMemoryGroup();
                    var _MemoryGroup = _IMemoryGroup.ToArray()[0];
                    var PixelData = MemoryMarshal.AsBytes(_MemoryGroup.Span).ToArray();

                    memory.Write(BitConverter.GetBytes((ulong)PixelData.Length));

                    memory.Write(PixelData);
                }
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
    }
}
