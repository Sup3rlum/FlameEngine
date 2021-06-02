using System;
using System.IO;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Security.Cryptography;


namespace ShaderLibraryLinker
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length > 0)
            {

                if (args[0] == "-i")
                {
                    string t = args[1];

                    List<string> inputs = new List<string>();
                    inputs.AddRange(t.Split(","));

                    Console.WriteLine($"Listed {inputs.Count} input files");

                    for (int i = 0; i < inputs.Count; i++)
                    {
                        Linker.OmitBinary(BuildScript.Load(inputs[i]));
                    }
                }
            }
        }
    }
}
