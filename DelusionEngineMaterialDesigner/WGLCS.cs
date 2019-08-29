using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace DelusionEngineMaterialDesigner
{
    public class WGLCS
    {

        [DllImport(@"D:\Projects\DelusionEngine\x64\Release\wglcs.dll", EntryPoint = "wglcsCreateGLContextFromHandle", CharSet = CharSet.Auto)]
        public static extern IntPtr CreateGLContextFromHandle(IntPtr _hWnd, int major, int minor);

        [DllImport(@"D:\Projects\DelusionEngine\x64\Release\wglcs.dll", EntryPoint = "wglcsDeleteContext", CharSet = CharSet.Auto)]
        public static extern void DeleteContext(IntPtr hrc);

        [DllImport(@"D:\Projects\DelusionEngine\x64\Release\wglcs.dll", EntryPoint = "wglcsSwapBuffers", CharSet = CharSet.Auto)]
        public static extern void SwapBuffers(IntPtr hdc);

        [DllImport(@"D:\Projects\DelusionEngine\x64\Release\wglcs.dll", EntryPoint = "wglcsClear", CharSet = CharSet.Auto)]
        public static extern void Clear(float r, float g, float b, float a);

        [DllImport(@"User32.dll", EntryPoint = "GetDC", CharSet = CharSet.Auto)]
        public static extern IntPtr GetDC(IntPtr hwnd);

        [DllImport(@"D:\Projects\DelusionEngine\x64\Release\wglcs.dll", EntryPoint = "wglcsCreateShader", CharSet = CharSet.Auto)]
        public static extern uint CreateShader(uint type, [MarshalAs(UnmanagedType.LPStr)]string str);

        [DllImport(@"D:\Projects\DelusionEngine\x64\Release\wglcs.dll", EntryPoint = "wglcsCreateProgram", CharSet = CharSet.Auto)]
        public static extern uint CreateProgram(uint vert, uint frag);

    }
}
