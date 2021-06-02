using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ShaderLibraryLinker
{
    public enum LibraryType
    {
        SPIRV = 0,
        DXIL = 1
    }

    public enum ShaderType
    {
        Vertex,
        Pixel,
        Geometry,
        Hull,
        Domain,
        Compute
    }

    public class ShaderLibraryModulePart
    {
        public ShaderType type;
        public byte[] partBinary;

    }

    public class ShaderLibraryModule
    {
        public string Name;
        public Dictionary<ShaderType, ShaderLibraryModulePart> Shaders = new Dictionary<ShaderType, ShaderLibraryModulePart>();

        public int ComponentNum
        {
            get
            {
                int num = 0;

                foreach (var kv in Shaders)
                {
                    if (kv.Value != null)
                    {
                        num++;
                    }
                }

                return num;
            }
        }

    }

    public class ShaderLibrary
    {
        public LibraryType Type;
        public List<ShaderLibraryModule> Modules = new List<ShaderLibraryModule>();


    }
}
