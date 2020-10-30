using System;
using System.Collections.Generic;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

namespace AssetEncoderCommandline
{

    public enum AssetType
    {
        Obj
    }

    public class FileInfo
    {
        public string Input;
        public string Output;

        public AssetType Type;


        public FileInfo(string i, string o, AssetType t)
        {
            Input = i;
            Output = o;
            Type = t;
        }
    }
    public class FileQueue
    {
        static List<FileInfo> _internal = new List<FileInfo>();

        static Task _task;

        public static void Add(FileInfo f)
        {
            _internal.Add(f);
        }

        public static void BeginQueue()
        {
            /*_task = new Task(() => BeginQueueInternal());
            _task.Start();*/

            BeginQueueInternal();
        }
        static void BeginQueueInternal()
        {
            foreach (var fi in _internal)
            {

                Console.WriteLine($"Encoding \"{ fi.Input }\" to \"{ fi.Output }\" ..");

                Transcoder.Convert(fi.Input, fi.Output);

            }
        }
        public static void EndQueue()
        {
            
        }
    }
}
