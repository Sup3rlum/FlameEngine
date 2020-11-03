using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Threading.Tasks;


using ContentCompiler.Compilers;
using ContentCompiler.ImportScripts;

namespace AssetEncoderCommandline
{

    public enum AssetType
    {
        Obj
    }

    public class FileInfo
    {
        public string Input;
        public AssetType Type;


        public FileInfo(string i,  AssetType t)
        {
            Input = i;
            Type = t;
        }
    }
    public class FileQueue
    {
        static List<BuildScript> _internal = new List<BuildScript>();

        static Task _task;

        public static void Add(BuildScript f)
        {
            _internal.Add(f);
        }

        public static void BeginQueue()
        {

            BeginQueueInternal();
        }
        static void BeginQueueInternal()
        {
            foreach (var fi in _internal)
            {

                Console.WriteLine($"Executing task \"{ fi._filePath }\" ...");

                fi.Load();
                fi.Execute();

            }
        }
        public static void EndQueue()
        {
            
        }
    }
}
