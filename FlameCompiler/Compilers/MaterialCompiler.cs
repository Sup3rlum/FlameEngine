using System;
using System.IO;
using System.IO.Compression;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;
using System.Security.Cryptography;
using System.Linq;
using System.Runtime.InteropServices;
using System.Drawing;
using System.Drawing.Imaging;

using FlameCompiler.Data;
using FlameCompiler.Math;

using FlameCompiler.ImportScripts;

using Ionic.Zlib;

using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Formats;
using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp.Processing;

namespace FlameCompiler.Compilers
{
    public class MaterialCompiler : ITaskCompiler
    {
        readonly byte[] Version = new byte[] { 1, 0, 0, 0 };
        readonly byte[] Signature = { (byte)'F', (byte)'L', (byte)'M', (byte)'T' };


        public MaterialCompiler()
        {
        }


        public void StartTask(BuildScriptTask task)
        {
            MemoryStream memory = new MemoryStream();

            EncodeMaterial((Material)task.Source, memory);
            var signedMemory = SignBinary(memory);
            OmitBinary(signedMemory, task.OutputFileName);
        }


        private void EncodeMaterial(Material material, MemoryStream memory)
        {
            // Diffuse Map
            var diffuse = material.MaterialMaps[MaterialMapType.Diffuse].Data[0];
            var data = ImageToByteArray32(diffuse);
            WriteMap(memory, diffuse.Width, diffuse.Height, data);


            // Normal Map
            var normal = material.MaterialMaps[MaterialMapType.Normal].Data[0];
            data = ImageToByteArray32(normal);
            WriteMap(memory, normal.Width, normal.Height, data);

            // Detail
            var height = material.MaterialMaps[MaterialMapType.Height].Data[0];
            var rough = material.MaterialMaps[MaterialMapType.Roughness].Data[0];
            var metal = material.MaterialMaps[MaterialMapType.Metallic].Data[0];
            var ao = material.MaterialMaps[MaterialMapType.AmbientOcclusion].Data[0];

            int max_width = 
                System.Math.Max(
                    System.Math.Max(
                        height.Width, 
                        rough.Width
                        ), 
                    System.Math.Max(
                        metal.Width, 
                        ao.Width));
            int max_height =
                System.Math.Max(
                    System.Math.Max(
                        height.Height,
                        rough.Height
                        ),
                    System.Math.Max(
                        metal.Height,
                        ao.Height));

            height = ResizeImage(height, max_width, max_height);
            rough = ResizeImage(rough, max_width, max_height);
            metal = ResizeImage(metal, max_width, max_height);
            ao = ResizeImage(ao, max_width, max_height);


            data = FromChannels(
                ImageToByteArray32(height),
                ImageToByteArray32(rough),
                ImageToByteArray32(metal),
                ImageToByteArray32(ao)
                );

            WriteMap(memory, max_width, max_height, data);


            var emissive = material.MaterialMaps[MaterialMapType.Emissive].Data[0];
            data = ImageToByteArray32(emissive);
            WriteMap(memory, emissive.Width, emissive.Height, data);

        }

        private MemoryStream SignBinary(MemoryStream memory)
        {
            // --- Checksum
            var data = memory.ToArray();
            var compressedData = data;// ZlibStream.CompressBuffer(data);
            var checksum = MD5.Create().ComputeHash(data);

            MemoryStream signedMemory = new MemoryStream();

            /* Header */

            signedMemory.Write(Signature);                                      // Signature    
            signedMemory.Write(Version);                                        // Version
            signedMemory.Write(checksum);                                       // Checksum

            /* Data */

            //signedMemory.Write(BitConverter.GetBytes(data.Length));             // Uncompressed Size  
            //signedMemory.Write(BitConverter.GetBytes(compressedData.Length));   // Compressed Size  
            signedMemory.Write(compressedData);                                 // Data

            return signedMemory;
        }
        private void OmitBinary(MemoryStream memory, string outputFile)
        {
            using (var outFileStream = new FileStream(outputFile, FileMode.Create))
            {
                memory.WriteTo(outFileStream);
            }
        }

        public byte[] ImageToByteArray32(Image<Rgba64> imageIn)
        {
            var img = imageIn.CloneAs<Rgba32>();

            Rgba32[] pixels = new Rgba32[imageIn.Width * imageIn.Height];
            img.CopyPixelDataTo(pixels);
            return MemoryMarshal.AsBytes<Rgba32>(pixels).ToArray();
        }
        public byte[] ImageToByteArray64(Image<Rgba64> img)
        {
            Rgba64[] pixels = new Rgba64[img.Width * img.Height];

            img.CopyPixelDataTo(pixels);
            return MemoryMarshal.AsBytes<Rgba64>(pixels).ToArray();
        }

        public byte[] FromChannels(byte[] srcRed, byte[] srcGreen, byte[] srcBlue, byte[] srcAlpha)
        {
            byte[] result = new byte[srcRed.Length];

            for (int i = 0; i < srcRed.Length / 4; i++)
            {
                result[4 * i] = srcRed[4 * i];
                result[4 * i + 1] = srcGreen[4 * i];
                result[4 * i + 2] = srcBlue[4 * i];
                result[4 * i + 3] = srcAlpha[4 * i];
            }
            return result;
        }

        void WriteMap(MemoryStream memory, int width, int height, byte[] dataToWrite)
        {

            //Image img = ImgFromBytes(dataToWrite, width, height);
            byte[] PixelData = dataToWrite;// GetPNGBytes(img);

            memory.Write(BitConverter.GetBytes(width));   // Map Width
            memory.Write(BitConverter.GetBytes(height));  // Map Height

            memory.Write(BitConverter.GetBytes((ulong)PixelData.Length));
            memory.Write(PixelData);
        }

        public Image<Rgba64> ResizeImage(Image<Rgba64> image, int width, int height)
        {
            if (image.Width != width || image.Height != height)
            {
                image.Mutate(x => x.Resize(width, height));
            }

            return image;
        }

       /* public Image ImgFromBytes(byte[] bytes, int width, int height)
        {
            /*
            Bitmap newbmp = new Bitmap(width, height, PixelFormat.Format32bppArgb);


            var data = newbmp.LockBits(
                new Rectangle(Point.Empty, newbmp.Size),
                ImageLockMode.ReadWrite, 
                newbmp.PixelFormat);

            Marshal.Copy(bytes, 0, data.Scan0, bytes.Length);

            newbmp.UnlockBits(data);

           // return newbmp;
        }*/
       /* public byte[] GetPNGBytes(Image imageIn)
        {
            using (var ms = new MemoryStream())
            {

                imageIn.Save(ms, ImageFormat.Png);
                return ms.ToArray();
            }
        }*/
    }
}
