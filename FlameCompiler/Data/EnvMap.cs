using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Text;
using System.Linq;
using System.Runtime.InteropServices;
using System.Drawing;

using FlameCompiler.Compilers;

namespace FlameCompiler.Data
{

    public enum EnvMapFaceName : int
    {
        IrrPosX,
        IrrNegX,
        IrrPosY,
        IrrNegY,
        IrrPosZ,
        IrrNegZ,


        SpecPosX,
        SpecNegX,
        SpecPosY,
        SpecNegY,
        SpecPosZ,
        SpecNegZ
    }


    public class EnvMap : ITextureMapCollection
    {
        private EnvMapFaceName GetFaceName(string name) => name switch
        {
            "IrrPosX" => EnvMapFaceName.IrrPosX,
            "IrrPosY" => EnvMapFaceName.IrrPosY,
            "IrrPosZ" => EnvMapFaceName.IrrPosZ,
            "IrrNegX" => EnvMapFaceName.IrrNegX,
            "IrrNegY" => EnvMapFaceName.IrrNegY,
            "IrrNegZ" => EnvMapFaceName.IrrNegZ,
            "SpecPosX" => EnvMapFaceName.SpecPosX,
            "SpecPosY" => EnvMapFaceName.SpecPosY,
            "SpecPosZ" => EnvMapFaceName.SpecPosZ,
            "SpecNegX" => EnvMapFaceName.SpecNegX,
            "SpecNegY" => EnvMapFaceName.SpecNegY,
            "SpecNegZ" => EnvMapFaceName.SpecNegZ,
            _ => throw new Exception("Material Map type not recognized")
        };

        public Dictionary<EnvMapFaceName, TextureMap> Faces { get; set; }
        public ObservableCollection<TextureMap> Values { get;set;
        }

        public EnvMap()
        {
            Faces = new Dictionary<EnvMapFaceName, TextureMap>();
            Values = new ObservableCollection<TextureMap>();

        }

        public void AddMap(string name, TextureMap map)
        {
            var type = GetFaceName(name);
            Faces.Add(type, map);
            Values.Clear();

            foreach (var (t, value) in Faces)
            {
                Values.Add(value);
            }
        }

        public ITaskCompiler GetCompiler()
        {
            return new EnvMapCompiler();
        }

        public string TypeName { get=> "envmap"; }
    }
}
