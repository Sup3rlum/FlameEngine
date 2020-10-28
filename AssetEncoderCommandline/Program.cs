using System;
using System.Collections.Generic;

namespace AssetEncoderCommandline
{
    class Program
    {
        static void Main(string[] args)
        {
            if (args.Length > 0)
            {

                List<string> inputs = new List<string>();
                List<string> outputs = new List<string>();

                if (args[0] == "-i")
                {
                    string t = args[1];

                    inputs.AddRange(t.Split(","));
                }
                if (args[2] == "-o")
                {
                    string t = args[3];

                    outputs.AddRange(t.Split(","));
                }

                Console.WriteLine($"Listed {inputs.Count} input files");

                if (outputs.Count == inputs.Count)
                {
                    for (int i = 0; i < inputs.Count; i++)
                    {

                        FileQueue.Add(new FileInfo(inputs[i], outputs[i], AssetType.Obj));
                    }

                    FileQueue.BeginQueue();
                }
            }
        }
    }
}
