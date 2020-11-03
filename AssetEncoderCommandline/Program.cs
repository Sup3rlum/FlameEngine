using ContentCompiler.ImportScripts;
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

                if (args[0] == "-i")
                {
                    string t = args[1];

                }

                inputs.AddRange(args[1].Split(","));
  
                Console.WriteLine($"Listed {inputs.Count} input files");


                for (int i = 0; i < inputs.Count; i++)
                {
                    FileQueue.Add(new BuildScript(inputs[i]));
                }

                FileQueue.BeginQueue();
            }
        }
    }
}
