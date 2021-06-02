using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Security.Cryptography;

namespace ShaderLibraryLinker
{

    public struct LinkerTask
    {
        public ShaderLibrary library;
        public string outputFileName;
    }

    public class Linker
    {
        static byte[] Version = new byte[] { 1, 0, 0, 0 };
        static byte[] Signature = { (byte)'F', (byte)'L', (byte)'S', (byte)'L' };

        public static MemoryStream LinkLibrary(ShaderLibrary lib)
        {
            MemoryStream memory = new MemoryStream();


            memory.Write(BitConverter.GetBytes((int)lib.Type));
            memory.Write(BitConverter.GetBytes(lib.Modules.Count));

            foreach (var module in lib.Modules)
            {
                memory.Write(BitConverter.GetBytes(module.Name.Length));
                memory.Write(Encoding.ASCII.GetBytes(module.Name));
                memory.Write(BitConverter.GetBytes(module.ComponentNum));

                foreach (var kv in module.Shaders)
                {
                    memory.Write(BitConverter.GetBytes((int)kv.Value.type));
                    memory.Write(BitConverter.GetBytes((int)kv.Value.partBinary.Length));
                    memory.Write(kv.Value.partBinary);
                }

            }



            return memory;
        }

        private static MemoryStream SignBinary(MemoryStream memory)
        {

            // --- Checksum
            var dataMemory = memory.ToArray();
            var md5 = MD5.Create();
            var checksum = md5.ComputeHash(dataMemory);


            MemoryStream binMemory = new MemoryStream();

            // -- Header
            binMemory.Write(Signature);                                         
            binMemory.Write(Version);                                        
            binMemory.Write(checksum);                                       

            binMemory.Write(dataMemory);                                     

            return binMemory;
        }


        public static void OmitBinary(LinkerTask task)
        {
            var mem = LinkLibrary(task.library);
            var bin = SignBinary(mem);

            FileStream outFileStream = new FileStream(task.outputFileName, FileMode.Create);

            bin.WriteTo(outFileStream);

            outFileStream.Close();
        }
    }
}
