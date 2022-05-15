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

    public enum MaterialMapType : int
    {
        Diffuse = 0,
        Normal = 1,
        Height = 2,
        Roughness = 3,
        Metallic = 4,
        AmbientOcclusion = 5,
        Emissive = 6
    }


    public class Material : ITextureMapCollection
    {

        private MaterialMapType GetMaterialMapType(string name) => name switch
        {
            "Diffuse"   => MaterialMapType.Diffuse,
            "Normal"    => MaterialMapType.Normal,
            "Height"    => MaterialMapType.Height,
            "Roughness" => MaterialMapType.Roughness,
            "Metallic"  => MaterialMapType.Metallic,
            "AO"        => MaterialMapType.AmbientOcclusion,
            "Emissive"  => MaterialMapType.Emissive,
            _ => throw new Exception("Material Map type not recognized")
        };

        public Dictionary<MaterialMapType, TextureMap> MaterialMaps { get; set; }
        public ObservableCollection<TextureMap> Values
        {
            get;set;
        }

        public Material()
        {
            MaterialMaps = new Dictionary<MaterialMapType, TextureMap>();
            Values = new ObservableCollection<TextureMap>();
        }

        public void AddMap(string name, TextureMap map)
        {
            var type = GetMaterialMapType(name);
            MaterialMaps.Add(type, map);

            Values.Clear();
            
            foreach (var (t, value) in MaterialMaps)
            {
                Values.Add(value);
            }
        }

        public ITaskCompiler GetCompiler()
        {
            return new MaterialCompiler();
        }

        public string TypeName { get => "material"; }
    }
}
