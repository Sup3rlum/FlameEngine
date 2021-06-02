using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace FlameEditor
{
    public class EngineInterop
    {
        [DllImport("FlameEngine.dll")]
        public static extern IntPtr CreateEngineInstance(IntPtr hwnd, int x, int y, int width, int height);

        [DllImport("FlameEngine.dll")]
        public static extern void EngineFrame(IntPtr eng);

        [DllImport("FlameEngine.dll")]
        public static extern bool IsActive(IntPtr eng);

        [DllImport("FlameEngine.dll")]
        public static extern void Dispose(IntPtr eng);

        [DllImport("FlameEngine.dll")]
        public static extern void Run(IntPtr eng);

    }
}
