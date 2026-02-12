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
using System.Drawing.Imaging;

using FlameEncoder.Data;

using SixLabors.ImageSharp;
using SixLabors.ImageSharp.Formats;
using SixLabors.ImageSharp.PixelFormats;
using SixLabors.ImageSharp.Processing;
using System.Windows.Media.Media3D;
using System.Windows;

namespace FlameEncoder.Compilers
{

    public class EnvMapCompiler : ITaskCompiler
    {
        readonly byte[] Version = new byte[] { 1, 0, 0, 0 };
        readonly byte[] Signature = { (byte)'F', (byte)'L', (byte)'E', (byte)'N' };


        public EnvMapCompiler()
        {
        }

        public void StartTask(CompilerTask task)
        {
            MemoryStream memory = new MemoryStream();
            EncodeEnvMap((EnvMap)task.TextureMaps, memory);
            var signedMemory = SignBinary(memory);
            OmitBinary(signedMemory, task.OutputFileName);


        }

        public string[] FaceNames =
        {
            "+X",
            "-X",
            "+Y",
            "-Y",
            "+Z",
            "-Z"
        };


        private void EncodeEnvMap(EnvMap envmap, MemoryStream memory)
        {

            // Specular Original 
            for (int i = 0; i < 6; i++)
            {
                var map = envmap.Maps[$"Skymap {FaceNames[i]}"].Data[0];

                var data = ImageToByteArray<RgbaVector>(map);
                WriteMap(memory, map.Width, map.Height, data);

            }

            // Specular
            for (int i = 0; i < 6; i++)
            {
                var map = envmap.Maps[$"Specular {FaceNames[i]}"].Data[0];

                var data = ImageToByteArray<Rgba64>(map);
                WriteMap(memory, map.Width, map.Height, data);
            }

            // Irradiance
            for (int i = 0; i < 6; i++)
            {
                var map = envmap.Maps[$"Irradiance {FaceNames[i]}"].Data[0];

                var data = ImageToByteArray<Rgba64>(map);
                WriteMap(memory, map.Width, map.Height, data);
            }
        }

        private MemoryStream SignBinary(MemoryStream memory)
        {
            // --- Checksum
            var data = memory.ToArray();
            var compressedData = data;//ZlibStream.CompressBuffer(data);
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

        public byte[] ImageToByteArray<TPixel>(Image<RgbaVector> imageIn) where TPixel: unmanaged, IPixel<TPixel>
        {
            Image<TPixel> image = imageIn.CloneAs<TPixel>();
            TPixel[] pixels = new TPixel[image.Width * image.Height];
            image.CopyPixelDataTo(pixels);

            return MemoryMarshal.AsBytes<TPixel>(pixels).ToArray();
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


        public static Image<RgbaVector> ResizeImage(Image<RgbaVector> image, int width, int height)
        {
            Image<RgbaVector> newImg = image.Clone(i => i.Resize(width, height));
            return newImg;
        }


        /*
        public (EnvMapFaceName, float, float) GetFromSpherical(Vertex3 vert, float cbSize)
        {
            vert = Vertex3.Normalize(vert);
            float temp = MathF.MaxMagnitude(vert.X, vert.Y);
            Vertex3 vertOnCube = vert * (1.0f / MathF.MaxMagnitude(temp, vert.Z));

            Vertex3 imgSpace = vertOnCube * (cbSize / 2.0f) + new Vertex3(cbSize/ 2.0f, cbSize / 2.0f, cbSize / 2.0f);

            return vertOnCube switch
            {
                (_, _, > 0.999f) => (EnvMapFaceName.PosZ, imgSpace.X, imgSpace.Y),
                (_, _, < -0.999f) => (EnvMapFaceName.NegZ, imgSpace.X, imgSpace.Y),
                (_, > 0.999f, _) => (EnvMapFaceName.PosY, imgSpace.X, imgSpace.Z),
                (_, < -0.999f, _) => (EnvMapFaceName.NegY, imgSpace.X, imgSpace.Z),
                (> 0.999f, _, _) => (EnvMapFaceName.PosX, imgSpace.Z, imgSpace.Y),
                (< -0.999f, _, _) => (EnvMapFaceName.NegX, imgSpace.Z, imgSpace.Y),
            };
        }

        public Dictionary<EnvMapFaceName, Image> CreateIrradianceMap(Dictionary<EnvMapFaceName, Image> cubemap)
        {
            Dictionary<EnvMapFaceName, Image> irradiance = new Dictionary<EnvMapFaceName, Image>()
            {
                { EnvMapFaceName.PosX, new Bitmap(32,32, PixelFormat.Format32bppArgb) },
                { EnvMapFaceName.NegX, new Bitmap(32,32, PixelFormat.Format32bppArgb) },
                { EnvMapFaceName.PosY, new Bitmap(32,32, PixelFormat.Format32bppArgb) },
                { EnvMapFaceName.NegY, new Bitmap(32,32, PixelFormat.Format32bppArgb) },
                { EnvMapFaceName.PosZ, new Bitmap(32,32, PixelFormat.Format32bppArgb) },
                { EnvMapFaceName.NegZ, new Bitmap(32,32, PixelFormat.Format32bppArgb) },
            };


            for (int face = 0; face < 6; face++)
            {
                for (int x = 0; x < 32; x++)
                {
                    for (int y = 0; y < 32; y++)
                    {

                        Vertex3 irr = new Vertex3(0, 0, 0);

                        Vertex3 Normal = Vertex3.Normalize(GetVertexCube((EnvMapFaceName)face, x, y));

                        Vertex3 up = new Vertex3(0.0f, 1.0f, 0.0f);
                        var tmpCross = Vertex3.Cross(up, Normal);
                        if (tmpCross.X == 0 && tmpCross.Y == 0.0f && tmpCross.Z == 0.0f)
                        {
                            tmpCross = Vertex3.Cross(new Vertex3(0.177f, 1.0f, 0.0f), Normal);
                        }

                        Vertex3 right = Vertex3.Normalize(tmpCross);

                        if (float.IsNaN(right.X))
                        {
                            int a = 5;
                        }

                        tmpCross = Vertex3.Cross(Normal, right);
                        if (tmpCross.X == 0 && tmpCross.Y == 0.0f && tmpCross.Z == 0.0f)
                        {
                            tmpCross = Vertex3.Cross(Normal, right + new Vertex3(0,-0.2f, 0.03f));
                        }

                        up = Vertex3.Normalize(tmpCross);

                        float sampleDelta = 0.025f;
                        float nrSamples = 0.0f;
                        for (float phi = 0.0f; phi < 2.0 * MathF.PI; phi += sampleDelta)
                        {
                            for (float theta = 0.0f; theta < 0.5 * MathF.PI; theta += sampleDelta)
                            {
                                // spherical to cartesian (in tangent space)
                                Vertex3 tangentSample = new Vertex3(MathF.Sin(theta) * MathF.Cos(phi), MathF.Sin(theta) * MathF.Sin(phi), MathF.Cos(theta));
                                // tangent space to world
                                Vertex3 sampleVec = tangentSample.X * right + tangentSample.Y * up + tangentSample.Z * Normal;

                                (var sampleF, var sampleX, var sampleY) = GetFromSpherical(sampleVec, 256);
                                float dTheta = MathF.Cos(theta) * MathF.Sin(theta);

                                var pix = ((Bitmap)cubemap[sampleF]).GetPixel((int)sampleX, (int)sampleY);

                                irr = irr + new Vertex3((float)pix.R / 255.0f, (float)pix.G / 255.0f, (float)pix.B / 255.0f) * dTheta;
                                nrSamples++;
                            }
                        }

                        irr = MathF.PI * irr * (1.0f / (float)nrSamples);

                        ((Bitmap)irradiance[(EnvMapFaceName)face]).SetPixel(x, y, Color.FromArgb(255, (int)(irr.X * 255.0f), (int)(irr.Y * 255.0f), (int)(irr.Z * 255.0f)));
                    }
                }
            }


            return irradiance;
        }*/
    }
}
